#include "Arduino.h"
#include "Screen.h"

#ifndef DISPLAY_SENSORSCREEN_H
#define DISPLAY_SENSORSCREEN_H


class SensorScreen : public Screen {
protected:
    bool scanIsRunning = false;
    long scanStartedAt = 0;
public:
    void beforeStartScan();

    void afterStartScan();
};

#endif //DISPLAY_SENSORSCREEN_H
