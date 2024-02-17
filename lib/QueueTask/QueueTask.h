#include "Arduino.h"

#ifndef DISPLAY_QUEUE_H
#define DISPLAY_QUEUE_H

typedef void (* TaskHandle)( void * );

struct Task {
    TaskHandle handle;
    void * args;
};

class QueueTask {
private:
    QueueHandle_t instance;
    Task currentTask;
public:
    QueueTask();
    void addTask(Task *task);
    void run();
};
#endif //DISPLAY_QUEUE_H
