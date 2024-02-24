#include "SensorScreen.h"
#include "Arduino.h"

void SensorScreen::beforeStartScan() {
    scanIsRunning = true;
    scanStartedAt = millis();
}

void SensorScreen::afterStartScan() {
    scanIsRunning = false;
    scanStartedAt = 0;
    triggerEvent(EventOnRadioUse);
}