#include "UiControl.h"
#include "QueueTask.h"
#include "IotRadioControl.h"
#include "Security.h"
#include "TopicsContainer.h"
#include "FileProjectPreferences.h"
#include "ScreenFactory.h"
#include "NetworkFactory.h"
#include "WiFiConfigurator.h"
#include "TelemetryTopics.h"

#define SETTINGS_FILENAME "/project.txt"

ScreenFactory *screenFactory;

typedef struct {
    UiControl &uiControl;
} UiParameters;

typedef struct {
    QueueTask &queue;
} QueueParameters;

typedef struct {
    ProjectPreferences &preferences;
    Dispatcher &dispatcher;
    UiControl &uiControl;
    NetworkFactory &factory;
    Security &security;
} MQTTParameters;

long timeWithoutNetworkConnection;
long lastAttemptNetworkConnection;

long lastTelemetrySendTime;

void loopMqtt(void *data) {
    auto parameters = (MQTTParameters *) data;
    auto connectionTimeout = stoi(parameters->preferences.getConnectionTimeout()) * 1000;
    auto maxConnectionAttemptsBeforeRestart = stoi(parameters->preferences.getConnectionAttemptsBeforeRestart());
    auto dispatcher = parameters->dispatcher;
    auto simNetwork = parameters->factory.createSimNetwork();
    while (true) {
        long now = millis();
        parameters->security.listenSmsCommands(simNetwork->getModem());
        if (dispatcher.getNetworkConnectionAttempts() >= maxConnectionAttemptsBeforeRestart
            || dispatcher.getCloudConnectionAttempts() >= maxConnectionAttemptsBeforeRestart) {
            auto currentModeIsWifi = parameters->preferences.networkModeIsWifi();
            if (!currentModeIsWifi) {
                parameters->preferences.setNetworkMode(ProjectPreferences::WifiNetworkMode);
            }
            if (currentModeIsWifi) {
                parameters->preferences.setNetworkMode(ProjectPreferences::SimNetworkMode);
            }
            parameters->security.runCommand(SecurityCommand::Restart);
        }

        if (!dispatcher.networkIsConnected() && !timeWithoutNetworkConnection) {
            timeWithoutNetworkConnection = millis();
        }

        if (timeWithoutNetworkConnection && (now - timeWithoutNetworkConnection) > connectionTimeout &&
            (now - lastAttemptNetworkConnection) > connectionTimeout) {
            dispatcher.connectToNetwork();
            lastAttemptNetworkConnection = now;
        }

        if (dispatcher.networkIsConnected() && !dispatcher.cloudIsConnected()) {
            dispatcher.connectToCloud();
        }

        if (dispatcher.networkIsConnected()) {
            timeWithoutNetworkConnection = 0;
        }

        if (dispatcher.cloudIsConnected()) {
            if ((now - lastTelemetrySendTime) >= 120000) {
                TelemetryTopics topics(dispatcher.getPubSubClient());
                topics.sendTelemetry(*simNetwork);
                lastTelemetrySendTime = now;
            }
            dispatcher.loop();
        }

        screenFactory->getMainScreen().handleConnections(
                dispatcher.cloudIsConnected(),
                dispatcher.networkIsConnected()
        );
        vTaskDelay(1000);
    }
}

void loopDisplay(void *data) {
    auto parameters = (UiParameters *) data;
    while (true) {
        parameters->uiControl.render();
        vTaskDelay(5);
    }
}

void loopQueue(void *data) {
    auto parameters = (QueueParameters *) data;
    while (true) {
        parameters->queue.run();
        vTaskDelay(5);
    }
}


void setup() {
    disableCore0WDT();
    disableCore1WDT();
    Serial1.begin(115200, SERIAL_8N1, GPIO_NUM_18, GPIO_NUM_17);
    Serial.begin(115200);
    FileProjectPreferences preferences(SETTINGS_FILENAME);
    TaskScheduler taskScheduler;
    IoTRadioDetect detect(preferences, taskScheduler);
    IotRadioControl control(preferences, taskScheduler);
    NetworkFactory factory(preferences, Serial1);
    auto network = factory.createNetwork();
    WiFiConfigurator configurator;
    configurator.configure(network->getType());
    PubSubClient pubSubClient(network->getClient());
    Topic cmdTopic("/security/command", pubSubClient);
    Topic rcvTopic("/security/receive", pubSubClient);
    Topic alarmTopic("/security/alarm", pubSubClient);
    TopicsContainer topicsContainer({
            &cmdTopic,
            &rcvTopic
    });
    Dispatcher dispatcher(preferences, topicsContainer, *network, pubSubClient);
    QueueTask queue;
    Security security(detect, control, preferences, cmdTopic, rcvTopic, alarmTopic);
    screenFactory = new ScreenFactory(preferences, detect, queue, control);
    UiControl uiControl(stoi(preferences.getSecurityTimeout()) * 1000);
    uiControl.init();

    auto securityListenCb = [&security](int eventId) {
        security.listen();
    };
    auto touchCb = [&uiControl](int eventId) {
        uiControl.simulateTouch();
    };
    security.onEvent((int) SecurityEvent::EventOnGuard, [touchCb](int eventId) {
        touchCb(eventId);
        screenFactory->getLockScreen().goTo(true);
    });

    security.onEvent((int) SecurityEvent::EventOnDisarm, [touchCb, &security](int eventId) {
        touchCb(eventId);
        screenFactory->getMainScreen().goTo(true);
    });

    security.onEvent((int) SecurityEvent::EventOnAlarm, touchCb);
    security.onEvent((int) SecurityEvent::EventOnMute, touchCb);
    security.onEvent((int) SecurityEvent::EventOnRestart, [&uiControl] (int eventId) {
        uiControl.backlightOff(false);
    });
    screenFactory->getDetectSensorsScreen().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    screenFactory->getControlSensorsScreen().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    screenFactory->getGeneralSettingsScreen().onEvent((int) GeneralSettingsScreenEvent::EventOnUpdateSettings, [&security] (int eventId) {
        security.runCommand(SecurityCommand::Restart);
    });
    screenFactory->getLockScreen().onEvent((int) LockScreenEvent::EventOnLock, [&security](int eventId) {
        security.lockSystem(true);
        screenFactory->getLockScreen().goTo(false);
    });
    screenFactory->getLockScreen().onEvent((int) LockScreenEvent::EventOnUnlock, [&security](int eventId) {
        security.unlockSystem(true);
        screenFactory->getMainScreen().goTo(false);
    });
    uiControl.onEvent((int) UiControlEvent::EventOnBacklightOff, [](int eventId) {
        screenFactory->getLockScreen().goTo(false);
    });

    MQTTParameters mqttParameters = {preferences, dispatcher, uiControl, factory, security};
    UiParameters uiParameters = {uiControl};
    QueueParameters queueParameters = {queue};
    taskScheduler.addTask({
          "loopDisplay",
          loopDisplay,
          TaskPriority::Low,
          5000,
          (void *) &uiParameters
    });
    taskScheduler.addTask({
          "loopMqtt",
          loopMqtt,
          TaskPriority::Low,
          5000,
          (void *) &mqttParameters
    });
    taskScheduler.addTask({
          "loopQueue",
          loopQueue,
          TaskPriority::Low,
          3000,
          (void *) &queueParameters
    });

    taskScheduler.schedule();
    while (true);
}

void loop() {

}