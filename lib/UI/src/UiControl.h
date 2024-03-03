#include "lvgl.h"
#include <SPI.h>
#include "map"
#include "Arduino_GFX_Library.h"
#include "Wire.h"
#include "TAMC_GT911.h"
#include "UiEventHandler.h"
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
    UiEventHandler *eventHandler;
    long timeWithoutTouch = 0;
    bool backlightIsOn = false;
    int backlightTimeout;

    void toggleBacklight();

    void backlightOn();

    void backlightOff();

public:
    UiControl(ProjectPreferences *p, IoTRadioDetect *i, Dispatcher *d, QueueTask *q, IotRadioControl *ct, int bcklTimeout);

    void render();

    void init();

    UiEventHandler *getEventHandler();

    void withoutTouch();

    void hasTouch();
};

#endif
