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

uint8_t receiverAddress[] = {0x08, 0xD1, 0xF9, 0xEB, 0x02, 0x58};

long timeWithoutNetworkConnection;
long lastAttemptNetworkConnection;
bool receiverAdded = false;
void loopMqtt(void *) {
    while (true) {
        try {
            //logger->debug("stack size mqtt - " + to_string(uxTaskGetStackHighWaterMark(nullptr)));
            long now = millis();
            if (!dispatcher->networkIsConnected() && !timeWithoutNetworkConnection) {
                timeWithoutNetworkConnection = millis();
            }

            if (timeWithoutNetworkConnection && (now - timeWithoutNetworkConnection) > 3000 &&
                (now - lastAttemptNetworkConnection) > 3000) {
                dispatcher->connectToNetwork();
                lastAttemptNetworkConnection = now;
            }

            if (dispatcher->networkIsConnected() &&
                (!dispatcher->cloudIsConnected() || dispatcher->hasNeedReconnectCloud())) {
                dispatcher->connectToCloud();
            }

            if (!receiverAdded) {
                IoTRadio::addReceiver(receiverAddress);
                security->listen();
                receiverAdded = true;
            }

            if (dispatcher->networkIsConnected()) {
                timeWithoutNetworkConnection = 0;
            }

            if (dispatcher->cloudIsConnected()) {
                dispatcher->loop();
            }

            // Serial.println(ESP.getFreeHeap());
            eventHandler->getMainScreen()->handleConnections();
            vTaskDelay(1000);
        } catch (const exception &e) {
            preferences->set(LastError, e.what());
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
            preferences->set(LastError, e.what());
        }
    }
}

void setup() {
    logger = new SerialLogger(9600);
    preferences = new ProjectPreferences(logger);
    logger->debug("Last error");
    logger->debug(preferences->get(LastError, "empty"));
    ioTRadioDetect = new IoTRadioDetect(preferences, logger);
    iotRadioControl = new IotRadioControl(preferences, logger);
    auto securityTopic = new Topic("/security/command");
    topics.push_back(securityTopic);
    dispatcher = new Dispatcher(preferences, logger, topics);
    taskScheduler = new TaskScheduler(logger);
    queue = new QueueTask();
    uiControl = new UiControl(preferences, ioTRadioDetect, dispatcher, queue, iotRadioControl);
    uiControl->init();
    eventHandler = uiControl->getEventHandler();
    security = new Security(ioTRadioDetect, iotRadioControl, uiControl, preferences, securityTopic);
    taskScheduler->addTask({LoopDisplayTask, loopDisplay, TaskPriority::Low, 5000});
    taskScheduler->addTask({LoopMqttTask, loopMqtt, TaskPriority::Low, 5000});
    auto listenCb = [](Events e) {
        security->listen();
    };
    eventHandler->getDetectSensorsScreen()->onEvent(listenCb, EventOnRadioUse);
    eventHandler->getControlSensorsScreen()->onEvent(listenCb, EventOnRadioUse);

}

void loop() {
    try {
        taskScheduler->schedule();
    } catch (std::runtime_error error) {
        logger->debug(error.what());
    }
    queue->run();
}