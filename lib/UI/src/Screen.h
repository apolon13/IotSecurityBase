#include "Arduino.h"
#include <map>
#include <vector>

#ifndef DISPLAY_SCREEN_H
#define DISPLAY_SCREEN_H

using namespace std;

enum Events {
    EventOnAfterRadioUse
};

class Screen {
protected:
    std::map<Events, vector<function<void(Events e)>>> handlers;

    void triggerEvent(Events e);

public:
    void onEvent(const function<void(Events)>& handler, Events e);
};

#endif
