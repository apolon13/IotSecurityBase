#include "Arduino.h"
#include <map>
#include <vector>

#ifndef DISPLAY_EVENTABLEOBJECT_H
#define DISPLAY_EVENTABLEOBJECT_H

using namespace std;

class EventableObject {
protected:
    std::map<int, vector<function<void(int)>>> handlers;

    void triggerEvent(const int &id);

public:
    void onEvent(const int &id, const function<void(int)> &handler);
};

#endif //DISPLAY_EVENTABLEOBJECT_H
