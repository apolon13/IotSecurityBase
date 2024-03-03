#include "SerialLogger.h"
#include "Logger.h"
#include "UiControl.h"
#include "QueueTask.h"
#include "IotRadioControl.h"
#include "Security.h"

UiEventHandler *eventHandler;

typedef struct {
    UiControl *uiControl;
} UiParameters;

typedef struct {
    ProjectPreferences *preferences;
    UiControl *uiControl;
    Dispatcher *dispatcher;
} MQTTParameters;


long timeWithoutNetworkConnection;
long lastAttemptNetworkConnection;

void loopMqtt(void *data) {
    auto parameters = (MQTTParameters *) data;
    auto connectionTimeout = stoi(parameters->preferences->getConnectionTimeout()) * 1000;
    auto maxConnectionAttemptsBeforeRestart = stoi(parameters->preferences->getConnectionAttemptsBeforeRestart());
    auto dispatcher = parameters->dispatcher;
    while (true) {
        //logger->debug("stack size mqtt - " + to_string(uxTaskGetStackHighWaterMark(nullptr)));
        long now = millis();

        if (dispatcher->getNetworkConnectionAttempts() >= maxConnectionAttemptsBeforeRestart) {
            ESP.restart();
        }

        if (!dispatcher->networkIsConnected() && !timeWithoutNetworkConnection) {
            timeWithoutNetworkConnection = millis();
        }

        if (timeWithoutNetworkConnection && (now - timeWithoutNetworkConnection) > connectionTimeout &&
            (now - lastAttemptNetworkConnection) > connectionTimeout) {
            dispatcher->connectToNetwork();
            lastAttemptNetworkConnection = now;
        }

        if (dispatcher->networkIsConnected() &&
            (!dispatcher->cloudIsConnected())) {
            dispatcher->connectToCloud();
        }

        if (dispatcher->networkIsConnected()) {
            timeWithoutNetworkConnection = 0;
        }

        if (dispatcher->cloudIsConnected()) {
            dispatcher->loop();
        }

        //Serial.println(ESP.getFreeHeap());
        parameters->uiControl->getEventHandler()->getMainScreen()->handleConnections();
        vTaskDelay(1000);
    }
}

long lastDisplayLoop;

void loopDisplay(void *data) {
    auto parameters = (UiParameters *) data;
    while (true) {
        long currentMs = millis();
        if ((currentMs - lastDisplayLoop) >= 1000) {
            //logger->debug("stack size display - " + to_string(uxTaskGetStackHighWaterMark(nullptr)));
            lastDisplayLoop = currentMs;
        }
        parameters->uiControl->render();
        vTaskDelay(5);
    }
}


void setup() {

}

void loop() {
    auto logger = new SerialLogger(9600);
    auto preferences = new ProjectPreferences(logger);
    logger->debug("Last error");
    logger->debug(preferences->get(ProjectPreferences::LastError, "empty"));
    auto ioTRadioDetect = new IoTRadioDetect(preferences, logger);
    auto iotRadioControl = new IotRadioControl(preferences, logger);
    auto securityCmdTopic = new Topic("/security/command");
    auto securityRcvTopic = new Topic("/security/receive");
    auto dispatcher = new Dispatcher(preferences, logger, {securityCmdTopic, securityRcvTopic});
    auto queue = new QueueTask();
    auto uiControl = new UiControl(
            preferences,
            ioTRadioDetect,
            dispatcher,
            queue,
            iotRadioControl,
            stoi(preferences->getSecurityTimeout()) * 1000
    );
    uiControl->init();
    eventHandler = uiControl->getEventHandler();
    auto security = new Security(ioTRadioDetect, iotRadioControl, preferences, securityCmdTopic, securityRcvTopic);
    auto lockScreen = uiControl->getEventHandler()->getLockScreen();
    auto mainScreen = uiControl->getEventHandler()->getMainScreen();

    auto securityListenCb = [security](int eventId) {
        security->listen();
    };
    eventHandler->getDetectSensorsScreen()->onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    eventHandler->getControlSensorsScreen()->onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);

    eventHandler->getLockScreen()->onEvent((int) LockScreenEvent::EventOnLock, [lockScreen, security](int eventId) {
        security->lockSystem(true);
        lockScreen->goTo(false);
    });

    eventHandler->getLockScreen()->onEvent((int) LockScreenEvent::EventOnUnlock, [mainScreen, security](int eventId) {
        security->unlockSystem(true);
        mainScreen->goTo(false);
    });

    auto touchCb = [uiControl](int eventId) {
        uiControl->hasTouch();
    };
    security->onEvent((int) SecurityEvent::EventOnGuard, [lockScreen, touchCb](int eventId) {
        touchCb(eventId);
        lockScreen->goTo(true);
    });

    security->onEvent((int) SecurityEvent::EventOnDisarm, [mainScreen, touchCb, security](int eventId) {
        touchCb(eventId);
        security->unlockSystem(true);
        mainScreen->goTo(true);
    });

    security->onEvent((int) SecurityEvent::EventOnAlarm, touchCb);
    security->onEvent((int) SecurityEvent::EventOnMute, touchCb);

    uiControl->onEvent((int) UiControlEvent::EventOnBacklightOff, [lockScreen, security](int eventId) {
        security->lockSystem(true);
        lockScreen->goTo(false);
    });
    auto taskScheduler = new TaskScheduler();

    MQTTParameters mqttParameters = {preferences, uiControl, dispatcher};
    taskScheduler->addTask({"loopDisplay", loopDisplay, TaskPriority::Low, 5000, (void *) {uiControl}});
    taskScheduler->addTask({"loopMqtt", loopMqtt, TaskPriority::Low, 5000, (void *)&mqttParameters});

    try {
        taskScheduler->schedule();
    } catch (std::runtime_error error) {
        Serial.println(error.what());
    }

    queue->run();
}