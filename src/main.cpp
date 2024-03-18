#include "Logger.h"
#include "UiControl.h"
#include "QueueTask.h"
#include "IotRadioControl.h"
#include "Security.h"
#include "TopicsContainer.h"
#include "FileProjectPreferences.h"
#include "ScreenFactory.h"
#include "NetworkFactory.h"
#include "WiFiConfigurator.h"

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
} MQTTParameters;

typedef struct {
    NetworkFactory &factory;
    UiControl &uiControl;
} AnswerResetParameters;


long timeWithoutNetworkConnection;
long lastAttemptNetworkConnection;

void loopMqtt(void *data) {
    auto parameters = (MQTTParameters *) data;
    auto connectionTimeout = stoi(parameters->preferences.getConnectionTimeout()) * 1000;
    auto maxConnectionAttemptsBeforeRestart = stoi(parameters->preferences.getConnectionAttemptsBeforeRestart());
    auto dispatcher = parameters->dispatcher;
    while (true) {
        //Serial.println(uxTaskGetStackHighWaterMark(nullptr));
        long now = millis();

        if (dispatcher.getNetworkConnectionAttempts() >= maxConnectionAttemptsBeforeRestart
        || dispatcher.getCloudConnectionAttempts() >= maxConnectionAttemptsBeforeRestart) {
            auto currentModeIsWifi = parameters->preferences.networkModeIsWifi();
            if (!currentModeIsWifi) {
                parameters->preferences.setNetworkMode(ProjectPreferences::WifiNetworkMode);
            }
            if (currentModeIsWifi) {
                parameters->preferences.setNetworkMode(ProjectPreferences::SimNetworkMode);
            }
            parameters->uiControl.backlightOff(false);
            ESP.restart();
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
            dispatcher.loop();
        }

        //Serial.println(ESP.getFreeHeap());
        screenFactory->getMainScreen().handleConnections(
                dispatcher.cloudIsConnected(),
                dispatcher.networkIsConnected()
        );
        vTaskDelay(1000);
    }
}

void loopAnswerReset(void *data) {
    auto parameters = (AnswerResetParameters *) data;
    std::unique_ptr<SimNetwork> simNetwork = parameters->factory.createSimNetwork();
    while(true) {
        if (simNetwork->getModem().callAnswer()) {
            Serial.println("restart");
            parameters->uiControl.backlightOff(false);
            ESP.restart();
        }
        vTaskDelay(5000);
    }
}

long lastDisplayLoop;

void loopDisplay(void *data) {
    auto parameters = (UiParameters *) data;
    while (true) {
        long currentMs = millis();
        if ((currentMs - lastDisplayLoop) >= 1000) {
            //Serial.println(uxTaskGetStackHighWaterMark(nullptr));
            lastDisplayLoop = currentMs;
        }
        parameters->uiControl.render();
        vTaskDelay(5);
    }
}

void loopQueue(void *data) {
    auto parameters = (QueueParameters *) data;
    while (true) {
        //Serial.println(uxTaskGetStackHighWaterMark(nullptr));
        parameters->queue.run();
        vTaskDelay(5);
    }
}


void setup() {

}

void loop() {
    Serial1.begin(115200, SERIAL_8N1, GPIO_NUM_18, GPIO_NUM_17);
    Serial.begin(115200);
    Logger logger(Serial);
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
    screenFactory->getDetectSensorsScreen().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    screenFactory->getControlSensorsScreen().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    screenFactory->getGeneralSettingsScreen().onEvent((int) GeneralSettingsScreenEvent::EventOnUpdateSettings, [&uiControl] (int eventId) {
        uiControl.backlightOff(false);
        ESP.restart();
    });

    screenFactory->getLockScreen().onEvent((int) LockScreenEvent::EventOnLock, [&security](int eventId) {
        security.lockSystem(true);
        screenFactory->getLockScreen().goTo(false);
    });

    screenFactory->getLockScreen().onEvent((int) LockScreenEvent::EventOnUnlock, [&security](int eventId) {
        security.unlockSystem(true);
        screenFactory->getMainScreen().goTo(false);
    });

    auto touchCb = [&uiControl](int eventId) {
        uiControl.simulateTouch();
    };
    security.onEvent((int) SecurityEvent::EventOnGuard, [touchCb](int eventId) {
        touchCb(eventId);
        screenFactory->getLockScreen().goTo(true);
    });

    security.onEvent((int) SecurityEvent::EventOnDisarm, [touchCb, &security](int eventId) {
        touchCb(eventId);
        security.unlockSystem(true);
        screenFactory->getMainScreen().goTo(true);
    });

    security.onEvent((int) SecurityEvent::EventOnAlarm, touchCb);
    security.onEvent((int) SecurityEvent::EventOnMute, touchCb);

    uiControl.onEvent((int) UiControlEvent::EventOnBacklightOff, [&security](int eventId) {
        security.lockSystem(true);
        screenFactory->getLockScreen().goTo(false);
    });

    MQTTParameters mqttParameters = {preferences, dispatcher, uiControl};
    UiParameters uiParameters = {uiControl};
    QueueParameters queueParameters = {queue};
    AnswerResetParameters answerParameters = {factory, uiControl};
    taskScheduler.addTask({
        "loopDisplay",
        loopDisplay,
        TaskPriority::Low,
        3500,
        (void *) &uiParameters
    });
    taskScheduler.addTask({
        "loopMqtt",
        loopMqtt,
        TaskPriority::Low,
        3000,
        (void *) &mqttParameters
    });
    taskScheduler.addTask({
        "loopQueue",
        loopQueue,
        TaskPriority::Low,
        1500,
        (void *) &queueParameters
    });
    taskScheduler.addTask({
        "loopAnswerReset",
        loopAnswerReset,
        TaskPriority::Low,
        1500,
        (void *) &answerParameters
    });

    taskScheduler.schedule();
    while (true);
}