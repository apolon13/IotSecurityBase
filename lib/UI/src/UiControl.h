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


class UiControl {
private:
    UiEventHandler *eventHandler;
public:
    UiControl(ProjectPreferences *p, IoTRadioDetect *i, Dispatcher *d, QueueTask *q, IotRadioControl *ct);

    void render();

    void init();

    UiEventHandler *getEventHandler();
};

#endif
