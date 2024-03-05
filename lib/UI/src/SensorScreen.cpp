#include "SensorScreen.h"
#include "Arduino.h"

void SensorScreen::beforeScan() {
    scanIsRunning = true;
    scanStartedAt = millis();
}

void SensorScreen::afterScan() {
    scanIsRunning = false;
    scanStartedAt = 0;
    triggerEvent((int) SensorScreenEvent::EventOnAfterRadioUse);
}