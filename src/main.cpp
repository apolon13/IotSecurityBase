#include "UiControl.h"
#include "QueueTask.h"
#include "IotRadioControl.h"
#include "Security.h"
#include "FileStore.h"
#include "Screens.h"
#include "NetworkFactory.h"
#include "Telemetry.h"
#include "FlashStore.h"
#include "StoreFactory.h"
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
    Store &store;
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
    auto connectionTimeout = stoi(proxy->store.getConnectionTimeout()) * 1000;
    auto maxConnectionAttemptsBeforeRestart = stoi(proxy->store.getConnectionAttemptsBeforeRestart());

    while (true) {
        unsigned long now = millis();
        if (networkConnectionAttempts >= maxConnectionAttemptsBeforeRestart || cloudConnectionAttempts >= maxConnectionAttemptsBeforeRestart) {
            auto currentModeIsWifi = proxy->store.networkModeIsWifi();
            if (!currentModeIsWifi) {
                proxy->store.setNetworkMode(Store::WifiNetworkMode);
            }
            if (currentModeIsWifi) {
                proxy->store.setNetworkMode(Store::SimNetworkMode);
            }
            proxy->security.runCommand(SecurityCommand::Restart);
        }

        if (!proxy->network.isConnected() && !timeWithoutNetworkConnection) {
            timeWithoutNetworkConnection = millis();
        }

        if (timeWithoutNetworkConnection && (now - timeWithoutNetworkConnection) > connectionTimeout &&
            (now - lastAttemptNetworkConnection) > connectionTimeout) {
            if (proxy->network.getType() == NetworkType::WiFi) {
                WiFiCredentials creds {
                  proxy->store.get(Store::WifiSsid, ""),
                  proxy->store.get(Store::WifiPassword, ""),
                };
                proxy->network.connect((void *)&creds);
            }
            networkConnectionAttempts++;
            lastAttemptNetworkConnection = now;
        }

        if (proxy->network.isConnected() && !proxy->pubSub.isConnected()) {
            cloudConnectionAttempts++;
            proxy->pubSub.connect(MqttCredentials{
                    proxy->store.get(Store::MqttEntityId, ""),
                    proxy->store.get(Store::MqttPassword, ""),
                    proxy->store.get(Store::MqttUsername, ""),
                    proxy->store.get(Store::MqttIp, ""),
                    proxy->store.get(Store::MqttPort, "1883"),
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
    Serial.begin(115200);

    StoreFactory storeFactory(SETTINGS_FILENAME);
    auto store = storeFactory.create();
    NetworkFactory networkFactory;
    auto network = networkFactory.create(*store);
    TaskScheduler taskScheduler;
    IoTRadioDetect detect(*store, taskScheduler);
    IotRadioControl control(*store, taskScheduler);

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
    Security security(detect, control, *store, ps);
    ps.onRecv(PubSub::Command,[&security](const std::string &payload) {
        security.handleControl(payload, true);
    });

    Telemetry telemetry(ps);
    Ticker periodicTicker;
    periodicTicker.attach_ms(120000, sendTelemetry, &telemetry);

    QueueTask queue;
    screens = new Screens(*store, detect, queue, control);
    UiControl uiControl(stoi(store->getSecurityTimeout()) * 1000);
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

    MQTTProxy MQTTProxy = {*store, uiControl, security, ps, *network};
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