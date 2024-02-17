#include "lvgl.h"
#include <SPI.h>
#include "Arduino_GFX_Library.h"
#include "Wire.h"
#include "TAMC_GT911.h"
#include "UiEventHandler.h"
#include "UiMutex.h"
#include "QueueTask.h"

#ifndef DISPLAY_UICONTROL_H
#define DISPLAY_UICONTROL_H

class UiControl {
private:
    UiEventHandler *eventHandler;
public:
    UiControl(ProjectPreferences *p, IoTRadioSignal *i, TaskScheduler *t, Dispatcher *d, QueueTask *q);

    void render();

    void init();

    UiEventHandler *getEventHandler();
};

#endif //DISPLAY_UICONTROL_H
