#include <vector>
#include "Logger.h"

#ifndef DISPLAY_SCHEDULER_H
#define DISPLAY_SCHEDULER_H

using namespace std;

enum Tasks {
    AddSensorTask,
    LoopDisplayTask,
    LoopMqttTask
};

enum TaskPriority {
    Low = 1,
    Middle,
    High
};

typedef struct {
    Tasks name;
    TaskFunction_t func;
    TaskPriority priority;
    int stackDepth;
} TaskToSchedule;

class TaskScheduler {
protected:
    Logger *logger;
    std::vector<TaskToSchedule> tasksToSchedule;
    std::vector<string> tasksToDelete;
    std::vector<string> inRunning;

    static string convertTaskId(Tasks id) {
        switch (id) {
            case AddSensorTask:
                return "addSensor";
            case LoopDisplayTask:
                return "loopDisplay";
            case LoopMqttTask:
                return "loopMqtt";
        }
        throw std::invalid_argument("Invalid task id");
    }

public:
    TaskScheduler(Logger *l);

    void schedule();

    void scheduleTask(TaskToSchedule t);

    void deleteTask(Tasks t);
};

#endif
