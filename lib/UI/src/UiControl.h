#include "lvgl.h"
#include <SPI.h>
#include "Arduino_GFX_Library.h"
#include "Wire.h"
#include "TAMC_GT911.h"
#include "UiEventHandler.h"
#include "UiMutex.h"

#ifndef DISPLAY_UICONTROL_H
#define DISPLAY_UICONTROL_H

class UiControl {
private:
    UiEventHandler *eventHandler;
public:
    UiControl(ProjectPreferences *p, IoTRadioSignal *i, TaskScheduler *t, Dispatcher *d);

    void render();

    void init();

    UiEventHandler *getEventHandler();
};

#endif //DISPLAY_UICONTROL_H
