#include "IoTRadioDetect.h"
#include "Store.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_EDITDETECTSENSORSCREEN_H
#define DISPLAY_EDITDETECTSENSORSCREEN_H

class EditDetectSensorScreen : public Screen {
private:
    IoTRadioDetect &ioTRadioDetect;
    Store &store;
public:
    EditDetectSensorScreen(IoTRadioDetect &i, Store &p) : ioTRadioDetect(i), store(p) {};

    void loadSensor(lv_event_t *e);

    void saveSensorSettings(lv_event_t *e);
};

#endif
