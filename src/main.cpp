#include "Logger.h"
#include "UiControl.h"
#include "QueueTask.h"
#include "IotRadioControl.h"
#include "Security.h"
#include "TopicsContainer.h"
#include "FileProjectPreferences.h"
#include "SimNetwork.h"
#include "WiFiNetwork.h"

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
} MQTTParameters;


long timeWithoutNetworkConnection;
long lastAttemptNetworkConnection;

void loopMqtt(void *data) {
    auto parameters = (MQTTParameters *) data;
    auto connectionTimeout = stoi(parameters->preferences.getConnectionTimeout()) * 1000;
    auto maxConnectionAttemptsBeforeRestart = stoi(parameters->preferences.getConnectionAttemptsBeforeRestart());
    auto dispatcher = parameters->dispatcher;
    while (true) {
        //logger.debug("stack size mqtt - " + to_string(uxTaskGetStackHighWaterMark(nullptr)));
        long now = millis();

        if (dispatcher.getNetworkConnectionAttempts() >= maxConnectionAttemptsBeforeRestart) {
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

       // Serial.println(ESP.getFreeHeap());
        screenFactory->getMainScreen().handleConnections(
                dispatcher.cloudIsConnected(),
                dispatcher.networkIsConnected()
        );
        vTaskDelay(1000);
    }
}

long lastDisplayLoop;

void loopDisplay(void *data) {
    auto parameters = (UiParameters *) data;
    while (true) {
        long currentMs = millis();
        if ((currentMs - lastDisplayLoop) >= 1000) {
            //logger.debug("stack size display - " + to_string(uxTaskGetStackHighWaterMark(nullptr)));
            lastDisplayLoop = currentMs;
        }
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

}

void loop() {
    Serial1.begin(115200, SERIAL_8N1, GPIO_NUM_18, GPIO_NUM_17);
    Serial.begin(115200);
    FileProjectPreferences preferences("/project.txt");
    IoTRadioDetect detect(preferences);
    IotRadioControl control(preferences);
    Topic cmdTopic("/security/command");
    Topic rcvTopic("/security/receive");
    TopicsContainer topicsContainer({
        &cmdTopic,
        &rcvTopic
    });

    Network *network;
    if (preferences.networkModeIsWifi()) {
        network = new SimNetwork(preferences, Serial1);
    } else {
        network = new WiFiNetwork(preferences);
    }

    Dispatcher dispatcher(preferences, topicsContainer, *network);
    QueueTask queue;
    Security security(detect, control, preferences, cmdTopic, rcvTopic);
    screenFactory = new ScreenFactory(preferences, detect, queue, control);
    UiControl uiControl(stoi(preferences.getSecurityTimeout()) * 1000);
    uiControl.init();

    auto securityListenCb = [&security](int eventId) {
        security.listen();
    };
    screenFactory->getDetectSensorsScreen().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    screenFactory->getControlSensorsScreen().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    screenFactory->getGeneralSettingsScreen().onEvent((int) GeneralSettingsScreenEvent::EventOnUpdateSettings, [] (int eventId) {
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
        uiControl.hasTouch();
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

    TaskScheduler taskScheduler;
    MQTTParameters mqttParameters = {preferences, dispatcher};
    UiParameters uiParameters = {uiControl};
    QueueParameters queueParameters = {queue};
    taskScheduler.addTask({"loopDisplay", loopDisplay, TaskPriority::Low, 5000, (void *) &uiParameters});
    taskScheduler.addTask({"loopMqtt", loopMqtt, TaskPriority::Low, 5000, (void *) &mqttParameters});
    taskScheduler.addTask({"loopQueue", loopQueue, TaskPriority::Low, 3000, (void *) &queueParameters});
    taskScheduler.schedule();

    while (true);
}