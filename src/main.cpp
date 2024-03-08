#include "Logger.h"
#include "UiControl.h"
#include "QueueTask.h"
#include "IotRadioControl.h"
#include "Security.h"
#include "TopicsContainer.h"

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

        Serial.println(ESP.getFreeHeap());
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
    Serial.begin(9600);
    ProjectPreferences preferences;
    IoTRadioDetect detect(preferences);
    IotRadioControl control(preferences);
    Topic cmdTopic("/security/command");
    Topic rcvTopic("/security/receive");
    TopicsContainer topicsContainer({
        &cmdTopic,
        &rcvTopic
    });
    Dispatcher dispatcher(preferences, topicsContainer);
    QueueTask queue;
    Security security(detect, control, preferences, cmdTopic, rcvTopic);
    screenFactory = new ScreenFactory(preferences, detect, queue, control);
    UiControl uiControl(stoi(preferences.getSecurityTimeout()) * 1000);
    uiControl.init();
    auto lockScreen = screenFactory->getLockScreen();
    auto mainScreen = screenFactory->getMainScreen();

    auto securityListenCb = [&security](int eventId) {
        security.listen();
    };
    screenFactory->getDetectSensorsScreen().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    screenFactory->getControlSensorsScreen().onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);

    lockScreen.onEvent((int) LockScreenEvent::EventOnLock, [&lockScreen, &security](int eventId) {
        security.lockSystem(true);
        lockScreen.goTo(false);
    });

    lockScreen.onEvent((int) LockScreenEvent::EventOnUnlock, [&mainScreen, &security](int eventId) {
        security.unlockSystem(true);
        mainScreen.goTo(false);
    });

    auto touchCb = [&uiControl](int eventId) {
        uiControl.hasTouch();
    };
    security.onEvent((int) SecurityEvent::EventOnGuard, [&lockScreen, touchCb](int eventId) {
        touchCb(eventId);
        lockScreen.goTo(true);
    });

    security.onEvent((int) SecurityEvent::EventOnDisarm, [&mainScreen, touchCb, &security](int eventId) {
        touchCb(eventId);
        security.unlockSystem(true);
        mainScreen.goTo(true);
    });

    security.onEvent((int) SecurityEvent::EventOnAlarm, touchCb);
    security.onEvent((int) SecurityEvent::EventOnMute, touchCb);

    uiControl.onEvent((int) UiControlEvent::EventOnBacklightOff, [&lockScreen, &security](int eventId) {
        security.lockSystem(true);
        lockScreen.goTo(false);
    });

    TaskScheduler taskScheduler;
    MQTTParameters mqttParameters = {preferences, dispatcher};
    UiParameters uiParameters = {uiControl};
    QueueParameters queueParameters = {queue};
    taskScheduler.addTask({"loopDisplay", loopDisplay, TaskPriority::Low, 5000, (void *) &uiParameters});
    taskScheduler.addTask({"loopMqtt", loopMqtt, TaskPriority::Low, 5000, (void *) &mqttParameters});
    taskScheduler.addTask({"loopQueue", loopQueue, TaskPriority::Low, 3000, (void *) &queueParameters});

    try {
        taskScheduler.schedule();
    } catch (std::runtime_error error) {
        Serial.println(error.what());
    }

    while (true);
}