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
#include <Ticker.h>

#define SETTINGS_FILENAME "/project.txt"

Screens *screens;

typedef struct {
    UiControl &uiControl;
} UiProxy;

typedef struct {
    QueueTask &queue;
} QueueProxy;

typedef struct {
    ProjectPreferences &preferences;
    UiControl &uiControl;
    Security &security;
    PubSub &pubSub;
    Network &network;
} MQTTProxy;

void sendTelemetry(Telemetry *t) {
    t->send();
}


void loopMqtt(void *data) {
    unsigned long timeWithoutNetworkConnection;
    unsigned long lastAttemptNetworkConnection;
    int networkConnectionAttempts = 0;
    int cloudConnectionAttempts = 0;
    auto proxy = (MQTTProxy *) data;
    auto connectionTimeout = stoi(proxy->preferences.getConnectionTimeout()) * 1000;
    auto maxConnectionAttemptsBeforeRestart = stoi(proxy->preferences.getConnectionAttemptsBeforeRestart());

    while (true) {
        unsigned long now = millis();
        if (networkConnectionAttempts >= maxConnectionAttemptsBeforeRestart || cloudConnectionAttempts >= maxConnectionAttemptsBeforeRestart) {
            auto currentModeIsWifi = proxy->preferences.networkModeIsWifi();
            if (!currentModeIsWifi) {
                proxy->preferences.setNetworkMode(ProjectPreferences::WifiNetworkMode);
            }
            if (currentModeIsWifi) {
                proxy->preferences.setNetworkMode(ProjectPreferences::SimNetworkMode);
            }
            proxy->security.runCommand(SecurityCommand::Restart);
        }

        if (!proxy->network.isConnected() && !timeWithoutNetworkConnection) {
            timeWithoutNetworkConnection = millis();
        }

        if (timeWithoutNetworkConnection && (now - timeWithoutNetworkConnection) > connectionTimeout &&
            (now - lastAttemptNetworkConnection) > connectionTimeout) {
            proxy->network.connect();
            networkConnectionAttempts++;
            lastAttemptNetworkConnection = now;
        }

        if (proxy->network.isConnected() && !proxy->pubSub.isConnected()) {
            cloudConnectionAttempts++;
            proxy->pubSub.connect(MqttCredentials{
                    proxy->preferences.get(ProjectPreferences::MqttEntityId, ""),
                    proxy->preferences.get(ProjectPreferences::MqttPassword, ""),
                    proxy->preferences.get(ProjectPreferences::MqttUsername, ""),
                    proxy->preferences.get(ProjectPreferences::MqttIp, ""),
                    proxy->preferences.get(ProjectPreferences::MqttPort, "1883"),
            });
        }

        if (proxy->network.isConnected()) {
            timeWithoutNetworkConnection = 0;
            networkConnectionAttempts = 0;
        }

        if (proxy->pubSub.isConnected()) {
            cloudConnectionAttempts = 0;
            proxy->pubSub.listen();
        }

        screens->main().changeConnectionIcons(
                proxy->pubSub.isConnected(),
                proxy->network.isConnected()
        );
        vTaskDelay(1000);
    }
}

void loopDisplay(void *data) {
    auto proxy = (UiProxy *) data;
    while (true) {
        proxy->uiControl.render();
        vTaskDelay(5);
    }
}

void loopQueue(void *data) {
    auto proxy = (QueueProxy *) data;
    while (true) {
        proxy->queue.run();
        vTaskDelay(5);
    }
}

void setup() {
    disableCore0WDT();
    disableCore1WDT();
    Serial1.begin(115200, SERIAL_8N1, GPIO_NUM_18, GPIO_NUM_17);
    Serial.begin(115200);

    PreferencesFactory preferencesFactory(SETTINGS_FILENAME);
    auto preferences = preferencesFactory.createPreferences();
    NetworkFactory factory(*preferences, Serial1);
    auto network = factory.createNetwork();
    TaskScheduler taskScheduler;
    IoTRadioDetect detect(*preferences, taskScheduler);
    IotRadioControl control(*preferences, taskScheduler);

    std::map<int, Topic> topics{
            std::make_pair(PubSub::Command, Topic("/security/command")),
            std::make_pair(PubSub::Receive, Topic("/security/receive")),
            std::make_pair(PubSub::Alarm, Topic("/security/alarm")),
            std::make_pair(PubSub::ChipTelemetry, Topic("/security/telemetry/chip-temperature")),
            std::make_pair(PubSub::HeapTelemetry, Topic("/security/telemetry/free-heap")),
            std::make_pair(PubSub::PSRAMTelemetry, Topic("/security/telemetry/free-psram")),
            std::make_pair(PubSub::WiFiTelemetry, Topic("/security/telemetry/wifi-channel")),
    };
    PubSubClient psClient(network->getClient());
    PubSub ps(topics, psClient);
    Security security(detect, control, *preferences, ps);
    ps.onRecv(PubSub::Command,[&security](const std::string &payload) {
        security.handleControl(payload, true);
    });

    Telemetry telemetry(ps);
    Ticker periodicTicker;
    periodicTicker.attach_ms(120000, sendTelemetry, &telemetry);

    QueueTask queue;
    screens = new Screens(*preferences, detect, queue, control);
    UiControl uiControl(stoi(preferences->getSecurityTimeout()) * 1000);
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

    MQTTProxy MQTTProxy = {*preferences, uiControl, security, ps, *network};
    UiProxy UiProxy = {uiControl};
    QueueProxy QueueProxy = {queue};
    taskScheduler.addTask({
      "loopDisplay",
      loopDisplay,
      TaskPriority::Low,
      5000,
      (void *) &UiProxy
    });
    taskScheduler.addTask({
      "loopMqtt",
      loopMqtt,
      TaskPriority::Low,
      5000,
      (void *) &MQTTProxy
    });
    taskScheduler.addTask({
      "loopQueue",
      loopQueue,
      TaskPriority::Low,
      3000,
      (void *) &QueueProxy
    });

    taskScheduler.schedule();
    while (true);
}

void loop() {}