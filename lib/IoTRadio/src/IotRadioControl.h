#include "IoTRadio.h"
#include "ProjectPreferences.h"

#ifndef DISPLAY_IOTRADIOCONTROL_H
#define DISPLAY_IOTRADIOCONTROL_H

class IotRadioControl : public IoTRadio {
protected:
    ProjectPreferences::PreferencesKey getPreferencesConfigKey() override;

public:
    IotRadioControl(const ProjectPreferences& p) : IoTRadio(p) {
        loadCurrentSensors();
    };

    string extractGroup(const string& signal);
};

#endif
