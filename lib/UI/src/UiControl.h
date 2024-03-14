#include "lvgl.h"
#include <SPI.h>
#include "map"
#include "LovyanGFX.h"
#include "Wire.h"
#include "TAMC_GT911.h"
#include "ScreenFactory.h"
#include "UiMutex.h"
#include "QueueTask.h"
#include "IotRadioControl.h"

#ifndef DISPLAY_UICONTROL_H
#define DISPLAY_UICONTROL_H

enum class UiControlEvent {
    EventOnBacklightOff
};

class UiControl : public EventableObject {
private:
    long timeWithoutTouch = 0;
    bool backlightIsOn = false;
    int backlightTimeout;

    void toggleBacklight();

public:
    explicit UiControl(int bcklTimeout);

    void render();

    void init();

    void backlightOn();

    void backlightOff();

    void withoutTouch();

    void hasTouch();
};

#endif
