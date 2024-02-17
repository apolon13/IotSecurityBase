#include "SerialLogger.h"
#include "Logger.h"
#include "UiControl.h"
#include "QueueTask.h"

Dispatcher *dispatcher;
UiControl *uiControl;
UiEventHandler *eventHandler;
Logger *logger;
TaskScheduler *taskScheduler;
ProjectPreferences *preferences;
IoTRadioSignal *ioTRadioSignal;
QueueTask *queue;

uint8_t receiverAddress[] = {0x08, 0xD1, 0xF9, 0xEB, 0x02, 0x58};
#define I2S_DOUT      17
#define I2S_BCLK      42
#define I2S_LRC       18

long timeWithoutNetworkConnection;
long lastAttemptNetworkConnection;
bool receiverAdded = false;
void loopMqtt(void *) {
    while (true) {
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

        if (dispatcher->networkIsConnected() && (!dispatcher->cloudIsConnected() || dispatcher->hasNeedReconnectCloud())) {
            dispatcher->connectToCloud();
        }

        if (!receiverAdded) {
            ioTRadioSignal->addReceiver(receiverAddress);
            receiverAdded = true;
        }

        if (dispatcher->networkIsConnected()) {
            timeWithoutNetworkConnection = 0;
        }

        if (dispatcher->cloudIsConnected()) {
            dispatcher->loop();
        }

        Serial.print("Free memory - ");
        Serial.println(ESP.getFreeHeap());
        eventHandler->handleConnections();
        vTaskDelay(1000);
    }
}

long lastDisplayLoop;
void loopDisplay(void *) {
    while (true) {
        long currentMs = millis();
        if ((currentMs - lastDisplayLoop) >= 1000) {
            //   logger->debug("stack size display - " + to_string(uxTaskGetStackHighWaterMark(nullptr)));
            lastDisplayLoop = currentMs;
        }
        uiControl->render();
        vTaskDelay(5);
    }
}

void setup() {
    logger = new SerialLogger(9600);
    preferences = new ProjectPreferences(logger);
    ioTRadioSignal = new IoTRadioSignal(preferences, GPIO_NUM_38, logger);
    dispatcher = new Dispatcher(preferences, logger);
    taskScheduler = new TaskScheduler(logger);
    queue = new QueueTask();
    uiControl = new UiControl(preferences, ioTRadioSignal, taskScheduler, dispatcher, queue);
    uiControl->init();
    eventHandler = uiControl->getEventHandler();
    taskScheduler->scheduleTask({LoopDisplayTask, loopDisplay, TaskPriority::Low, 5000});
    taskScheduler->scheduleTask({LoopMqttTask, loopMqtt, TaskPriority::Low, 5000});
}

void loop() {
    try {
        taskScheduler->schedule();
    } catch (std::runtime_error error) {
        logger->debug(error.what());
    }
    queue->run();
}