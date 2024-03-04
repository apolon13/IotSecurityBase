#include "lvgl.h"
#include <SPI.h>
#include "map"
#include "Arduino_GFX_Library.h"
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

    void backlightOn();

    void backlightOff();

public:
    explicit UiControl(int bcklTimeout);

    void render();

    void init();

    void withoutTouch();

    void hasTouch();
};

#endif
