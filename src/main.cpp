#include "UiControl.h"
#include "QueueTask.h"
#include "IotRadioControl.h"
#include "Security.h"
#include "FileProjectPreferences.h"
#include "Screens.h"
#include "NetworkFactory.h"
#include "Telemetry.h"
#include "FlashProjectPreferences.h"
#include "PreferencesFactory.h"
#include "PubSub.h"

#define SETTINGS_FILENAME "/project.txt"

Screens *screens;

typedef struct {
    UiControl &uiControl;
} UiParameters;

typedef struct {
    QueueTask &queue;
} QueueParameters;

typedef struct {
    ProjectPreferences &preferences;
    UiControl &uiControl;
    Security &security;
    PubSub &pubSub;
    Network &network;
} MQTTParameters;

long timeWithoutNetworkConnection;
long lastAttemptNetworkConnection;
long lastTelemetrySendTime;

void loopMqtt(void *data) {
    auto parameters = (MQTTParameters *) data;
    auto connectionTimeout = stoi(parameters->preferences.getConnectionTimeout()) * 1000;
    auto maxConnectionAttemptsBeforeRestart = stoi(parameters->preferences.getConnectionAttemptsBeforeRestart());
    Telemetry telemetry(parameters->pubSub);
    while (true) {
        long now = millis();

        if (!parameters->network.isConnected() && !timeWithoutNetworkConnection) {
            timeWithoutNetworkConnection = millis();
        }

        if (timeWithoutNetworkConnection && (now - timeWithoutNetworkConnection) > connectionTimeout &&
            (now - lastAttemptNetworkConnection) > connectionTimeout) {
            parameters->network.connect();
            lastAttemptNetworkConnection = now;
        }

        if (parameters->network.isConnected() && !parameters->pubSub.isConnected()) {
            parameters->pubSub.connect(MqttCredentials{
                    parameters->preferences.get(ProjectPreferences::MqttEntityId, ""),
                    parameters->preferences.get(ProjectPreferences::MqttPassword, ""),
                    parameters->preferences.get(ProjectPreferences::MqttUsername, ""),
                    parameters->preferences.get(ProjectPreferences::MqttIp, ""),
                    parameters->preferences.get(ProjectPreferences::MqttPort, "1883"),
            });
        }

        if (parameters->network.isConnected()) {
            timeWithoutNetworkConnection = 0;
        }

        if (parameters->pubSub.isConnected()) {
            if ((now - lastTelemetrySendTime) >= 120000) {
                telemetry.send();
                lastTelemetrySendTime = now;
            }
            parameters->pubSub.listen();
        }

        screens->main().handleConnections(
                parameters->pubSub.isConnected(),
                parameters->network.isConnected()
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
    NetworkFactory factory(preferences, Serial1);
    auto network = factory.createNetwork();
    TaskScheduler taskScheduler;
    IoTRadioDetect detect(preferences, taskScheduler);
    IotRadioControl control(preferences, taskScheduler);

    std::map<int, Topic> topics{
            std::make_pair(PubSub::Command, Topic("/security/command")),
            std::make_pair(PubSub::Receive, Topic("/security/receive")),
            std::make_pair(PubSub::Alarm, Topic("/security/alarm")),
            std::make_pair(PubSub::ChipTelemetry, Topic("/security/telemetry/chip-temperature")),
            std::make_pair(PubSub::HeapTelemetry, Topic("/security/telemetry/free-heap")),
            std::make_pair(PubSub::PSRAMTelemetry, Topic("/security/telemetry/free-psram")),
            std::make_pair(PubSub::WiFiTelemetry, Topic("/security/telemetry/wifi-channel")),
    };
    PubSub ps(topics, *network);
    Security security(detect, control, preferences, ps);
    ps.onRecv(PubSub::Command,[&security](const std::string &payload) {
        security.handleControl(payload, true);
    });

    QueueTask queue;
    screens = new Screens(preferences, detect, queue, control);
    UiControl uiControl(stoi(preferences.getSecurityTimeout()) * 1000);
    uiControl.init();
    auto securityListenCb = [&security](int eventId) {
        security.listen();
    };

    auto touchCb = [&uiControl](int eventId) {
        uiControl.simulateTouch();
    };

    //SECURITY EVENTS
    security.onEvent((int) SecurityEvent::EventOnGuard, [touchCb](int eventId) {
        touchCb(eventId);
        screens->lock().goTo(true);
    });
    security.onEvent((int) SecurityEvent::EventOnDisarm, [touchCb, &security](int eventId) {
        touchCb(eventId);
        screens->main().goTo(true);
    });
    security.onEvent((int) SecurityEvent::EventOnAlarm, touchCb);
    security.onEvent((int) SecurityEvent::EventOnMute, touchCb);
    security.onEvent((int) SecurityEvent::EventOnRestart, [&uiControl](int eventId) {
        uiControl.backlightOff(false);
    });

    //UI EVENTS
    screens->detectSensors().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    screens->controlSensors().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    screens->generalSettings().onEvent((int) GeneralSettingsScreenEvent::EventOnUpdateSettings,
    [&security](int eventId) {
        security.runCommand(SecurityCommand::Restart);
    });
    screens->lock().onEvent((int) LockScreenEvent::EventOnLock, [&security](int eventId) {
        security.lockSystem(true);
        screens->lock().goTo(false);
    });
    screens->lock().onEvent((int) LockScreenEvent::EventOnUnlock, [&security](int eventId) {
        security.unlockSystem(true);
        screens->main().goTo(false);
    });
    uiControl.onEvent((int) UiControlEvent::EventOnBacklightOff, [](int eventId) {
        screens->lock().goTo(false);
    });

    MQTTParameters mqttParameters = {preferences, uiControl, security, ps, *network};
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