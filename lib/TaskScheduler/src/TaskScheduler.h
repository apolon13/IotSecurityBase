#include <vector>
#include "Logger.h"

#ifndef DISPLAY_SCHEDULER_H
#define DISPLAY_SCHEDULER_H

enum class TaskPriority {
    Low = 1,
    Middle,
    High
};

struct TaskToSchedule {
    std::string name;
    TaskFunction_t func;
    TaskPriority priority;
    int stackDepth;
    void *parameters;
};

class TaskScheduler {
protected:
    std::vector<TaskToSchedule> tasksToSchedule;
    std::vector<std::string> tasksToDelete;
    std::vector<std::string> inRunning;

public:
    explicit TaskScheduler();

    void schedule();

    void addTask(const TaskToSchedule &t);

    void deleteTask(const std::string &name);
};

#endif
