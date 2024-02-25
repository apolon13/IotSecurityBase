#include "TaskScheduler.h"

void TaskScheduler::addTask(const TaskToSchedule& t) {
    bool exist = false;
    for (const auto& task: inRunning) {
        if (task == t.name) {
            exist = true;
            break;
        }
    }
    if (!exist) {
        tasksToSchedule.push_back(t);
    }
}

TaskScheduler::TaskScheduler(Logger *l): logger(l) {

}

long lastInfo;
void TaskScheduler::schedule() {
    long currentMs = millis();
    for (auto task: tasksToSchedule) {
        auto handle = xTaskGetHandle(task.name.c_str());
        if (handle == nullptr) {
            auto code = xTaskCreate(
                    task.func,
                    task.name.c_str(),
                    task.stackDepth,
                    nullptr,
                    (int)task.priority,
                    nullptr
            );
            if (code != pdPASS) {
                throw std::runtime_error("Create task error " + to_string(code));
            }
        } else {
            vTaskResume(handle);
        }
        inRunning.push_back(task.name);
    }
    tasksToSchedule.clear();

    for (const auto& task: tasksToDelete) {
        auto handle = xTaskGetHandle(task.c_str());
        vTaskSuspend(handle);
        auto iterator = find(inRunning.begin(), inRunning.end(), task);
        if (iterator != inRunning.end()) {
            inRunning.erase(iterator);
        }
    }
    tasksToDelete.clear();

    if ((currentMs - lastInfo) >= 5000) {
        for (const auto& task : inRunning) {
            auto handle = xTaskGetHandle(task.c_str());
            if (handle != nullptr) {
                auto status = eTaskGetState(handle);
               // logger->debug("Task " + task);
               // logger->debug("Status " + to_string(status));
            } else {
              //  logger->debug(task + " not found");
            }
        }
        lastInfo = currentMs;
    }
}

void TaskScheduler::deleteTask(const string& name) {
    for (const auto& task: inRunning) {
        if (task == name) {
            tasksToDelete.push_back(task);
            break;
        }
    }
}
