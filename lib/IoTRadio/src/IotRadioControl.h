#include "IoTRadio.h"
#include "ProjectPreferences.h"

#ifndef DISPLAY_IOTRADIOCONTROL_H
#define DISPLAY_IOTRADIOCONTROL_H
class IotRadioControl: public IoTRadio {
protected:
    Props getPreferencesConfigKey() override;
public:
    IotRadioControl(ProjectPreferences *p, Logger *l): IoTRadio(p, l) {
        loadCurrentSensors();
    };

    string extractGroup(string signal);
};
#endif
