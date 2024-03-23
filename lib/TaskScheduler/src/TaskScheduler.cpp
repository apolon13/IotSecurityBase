#include "TaskScheduler.h"

using namespace std;

void TaskScheduler::addTask(const TaskToSchedule &t) {
    bool exist = false;
    for (const auto &task: inRunning) {
        if (task == t.name) {
            exist = true;
            break;
        }
    }
    if (!exist) {
        tasksToSchedule.push_back(t);
    }
}

TaskScheduler::TaskScheduler() {

}

void TaskScheduler::schedule() {
    for (auto task: tasksToSchedule) {
        TaskHandle_t handle = getTaskHandle(task.name);
        if (handle == nullptr) {
            int code;
            if (task.core != -1) {
                code = xTaskCreatePinnedToCore(
                        task.func,
                        task.name.c_str(),
                        task.stackDepth,
                        task.parameters,
                        (int) task.priority,
                        nullptr,
                        task.core
                );
            } else {
                code = xTaskCreate(
                        task.func,
                        task.name.c_str(),
                        task.stackDepth,
                        task.parameters,
                        (int) task.priority,
                        nullptr
                );
            }
            if (code != pdPASS) {
                throw std::runtime_error("Create task error " + to_string(code));
            }
        } else {
            vTaskResume(handle);
        }
        inRunning.push_back(task.name);
    }
    tasksToSchedule.clear();

    for (const auto &task: tasksToDelete) {
        auto handle = xTaskGetHandle(task.c_str());
        vTaskSuspend(handle);
        auto iterator = find(inRunning.begin(), inRunning.end(), task);
        if (iterator != inRunning.end()) {
            inRunning.erase(iterator);
        }
    }
    tasksToDelete.clear();
}

void TaskScheduler::deleteTask(const string &name) {
    for (const auto &task: inRunning) {
        if (task == name) {
            tasksToDelete.push_back(task);
            break;
        }
    }
}

TaskHandle_t TaskScheduler::getTaskHandle(const std::string& taskName) {
    return xTaskGetHandle(taskName.c_str());
}
