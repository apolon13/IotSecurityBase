#include "Arduino.h"
#include <map>
#include <vector>

#ifndef DISPLAY_EVENTABLEOBJECT_H
#define DISPLAY_EVENTABLEOBJECT_H

class EventableObject {
protected:
    std::map<int, std::vector<std::function<void(int)>>> handlers;

    void triggerEvent(const int &id);

public:
    void onEvent(const int &id, const std::function<void(int)> &handler);
};

#endif //DISPLAY_EVENTABLEOBJECT_H
