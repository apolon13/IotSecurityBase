#include "Arduino.h"
#include "Screen.h"

#ifndef DISPLAY_SENSORSCREEN_H
#define DISPLAY_SENSORSCREEN_H

enum class SensorScreenEvent{
    EventOnAfterRadioUse
};

class SensorScreen : public Screen {
protected:
    bool scanIsRunning = false;
    long scanStartedAt = 0;
public:
    void beforeScan();

    void afterScan();
};

#endif
