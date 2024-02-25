#include "Arduino.h"
#include <map>
#include <vector>

#ifndef DISPLAY_SCREEN_H
#define DISPLAY_SCREEN_H

using namespace std;

class Screen {
protected:
    std::map<int, vector<function<void(int)>>> handlers;

    void triggerEvent(const int& id);

public:
    void onEvent(const function<void(int)>& handler, const int& id);
};

#endif
