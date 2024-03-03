#include <vector>
#include "Logger.h"

#ifndef DISPLAY_SCHEDULER_H
#define DISPLAY_SCHEDULER_H

using namespace std;

enum class TaskPriority {
    Low = 1,
    Middle,
    High
};

struct TaskToSchedule {
    string name;
    TaskFunction_t func;
    TaskPriority priority;
    int stackDepth;
    void *parameters;
};

class TaskScheduler {
protected:
    std::vector<TaskToSchedule> tasksToSchedule;
    std::vector<string> tasksToDelete;
    std::vector<string> inRunning;

public:
    explicit TaskScheduler();

    void schedule();

    void addTask(const TaskToSchedule& t);

    void deleteTask(const string& name);
};

#endif
