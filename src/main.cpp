#include "SerialLogger.h"
#include "Logger.h"
#include "UiControl.h"
#include "QueueTask.h"
#include "IotRadioControl.h"
#include "Security.h"

Dispatcher *dispatcher;
UiControl *uiControl;
UiEventHandler *eventHandler;
Logger *logger;
TaskScheduler *taskScheduler;
Security *security;
ProjectPreferences *preferences;
IoTRadioDetect *ioTRadioDetect;
QueueTask *queue;
IotRadioControl *iotRadioControl;
vector<Topic *> topics;

long timeWithoutNetworkConnection;
long lastAttemptNetworkConnection;

void loopMqtt(void *) {
    while (true) {
        try {
            //logger->debug("stack size mqtt - " + to_string(uxTaskGetStackHighWaterMark(nullptr)));
            long now = millis();

            if (dispatcher->getNetworkConnectionAttempts() > 10) {
                ESP.restart();
            }

            if (!dispatcher->networkIsConnected() && !timeWithoutNetworkConnection) {
                timeWithoutNetworkConnection = millis();
            }

            if (timeWithoutNetworkConnection && (now - timeWithoutNetworkConnection) > 3000 &&
                (now - lastAttemptNetworkConnection) > 3000) {
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
            eventHandler->getMainScreen()->handleConnections();
            vTaskDelay(1000);
        } catch (const exception &e) {
            preferences->set(ProjectPreferences::LastError, e.what());
        }
    }
}

long lastDisplayLoop;

void loopDisplay(void *) {
    while (true) {
        try {
            long currentMs = millis();
            if ((currentMs - lastDisplayLoop) >= 1000) {
                //logger->debug("stack size display - " + to_string(uxTaskGetStackHighWaterMark(nullptr)));
                lastDisplayLoop = currentMs;
            }
            uiControl->render();
            vTaskDelay(5);
        } catch (const exception &e) {
            preferences->set(ProjectPreferences::LastError, e.what());
        }
    }
}

void setup() {
    logger = new SerialLogger(9600);
    preferences = new ProjectPreferences(logger);
    logger->debug("Last error");
    logger->debug(preferences->get(ProjectPreferences::LastError, "empty"));
    ioTRadioDetect = new IoTRadioDetect(preferences, logger);
    iotRadioControl = new IotRadioControl(preferences, logger);
    auto securityCmdTopic = new Topic("/security/command");
    auto securityRcvTopic = new Topic("/security/receive");
    topics.push_back(securityCmdTopic);
    topics.push_back(securityRcvTopic);
    dispatcher = new Dispatcher(preferences, logger, topics);
    taskScheduler = new TaskScheduler(logger);
    queue = new QueueTask();
    uiControl = new UiControl(preferences, ioTRadioDetect, dispatcher, queue, iotRadioControl);
    uiControl->init();
    eventHandler = uiControl->getEventHandler();
    security = new Security(ioTRadioDetect, iotRadioControl, preferences, securityCmdTopic, securityRcvTopic);
    auto lockScreen = uiControl->getEventHandler()->getLockScreen();
    auto mainScreen = uiControl->getEventHandler()->getMainScreen();

    taskScheduler->addTask({"loopDisplay", loopDisplay, TaskPriority::Low, 5000});
    taskScheduler->addTask({"loopMqtt", loopMqtt, TaskPriority::Low, 5000});

    auto securityListenCb = [](int eventId) {
        security->listen();
    };
    eventHandler->getDetectSensorsScreen()->onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);
    eventHandler->getControlSensorsScreen()->onEvent((int) SensorScreenEvent::EventOnAfterRadioUse, securityListenCb);

    eventHandler->getLockScreen()->onEvent((int) LockScreenEvent::EventOnLock, [lockScreen](int eventId) {
        security->lockSystem(true);
        lockScreen->goTo(false);
    });

    eventHandler->getLockScreen()->onEvent((int) LockScreenEvent::EventOnUnlock, [mainScreen](int eventId) {
        security->unlockSystem(true);
        mainScreen->goTo(false);
    });

    auto touchCb = [](int eventId) {
        uiControl->hasTouch();
    };
    security->onEvent((int) SecurityEvent::EventOnGuard, [lockScreen, touchCb](int eventId) {
        touchCb(eventId);
        lockScreen->goTo(true);
    });

    security->onEvent((int) SecurityEvent::EventOnDisarm, [mainScreen, touchCb](int eventId) {
        touchCb(eventId);
        security->unlockSystem(true);
        mainScreen->goTo(true);
    });

    security->onEvent((int) SecurityEvent::EventOnAlarm, touchCb);
    security->onEvent((int) SecurityEvent::EventOnMute, touchCb);

    uiControl->onEvent((int) UiControlEvent::EventOnBacklightOff, [lockScreen](int eventId) {
        security->lockSystem(true);
        lockScreen->goTo(false);
    });
}

void loop() {
    try {
        taskScheduler->schedule();
    } catch (std::runtime_error error) {
        logger->debug(error.what());
    }
    queue->run();
}