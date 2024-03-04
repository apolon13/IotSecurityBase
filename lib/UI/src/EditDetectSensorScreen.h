#include "IoTRadioDetect.h"
#include "ProjectPreferences.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_EDITDETECTSENSORSCREEN_H
#define DISPLAY_EDITDETECTSENSORSCREEN_H

class EditDetectSensorScreen : public Screen {
private:
    IoTRadioDetect ioTRadioDetect;
    ProjectPreferences projectPreferences;
public:
    EditDetectSensorScreen(const IoTRadioDetect& i, const ProjectPreferences& p) : ioTRadioDetect(i), projectPreferences(p) {};

    void loadSensor(lv_event_t *e);

    void saveSensorSettings(lv_event_t *e);
};

#endif
