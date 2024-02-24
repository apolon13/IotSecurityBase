#include "Arduino.h"

#ifndef DISPLAY_QUEUE_H
#define DISPLAY_QUEUE_H

struct Task {
    std::function<void(void *data)> handle;
    void *args;
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
