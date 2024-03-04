#include "QueueTask.h"

QueueTask::QueueTask() {
    instance = xQueueCreate(10, sizeof(currentTask));
}

void QueueTask::addTask(Task *task) {
    xQueueSend(instance, task, portMAX_DELAY);
}

void QueueTask::run() {
    xQueueReceive(instance, &currentTask, 0);
    if (currentTask.handle != nullptr) {
        currentTask.handle(currentTask.args);
        currentTask = {};
    }
}
