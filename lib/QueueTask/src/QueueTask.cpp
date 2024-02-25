#include "QueueTask.h"

QueueTask::QueueTask() {
    instance = xQueueCreate(10, sizeof(currentTask));
}

void QueueTask::addTask(Task *task) {
    xQueueSend(instance, task, portMAX_DELAY);
}

void QueueTask::run() {
    xQueueReceive(instance, &currentTask, portMAX_DELAY);
    currentTask.handle(currentTask.args);
    currentTask = {};
}
