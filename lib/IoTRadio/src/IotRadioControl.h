#include "IoTRadio.h"
#include "ProjectPreferences.h"

#ifndef DISPLAY_IOTRADIOCONTROL_H
#define DISPLAY_IOTRADIOCONTROL_H

class IotRadioControl : public IoTRadio {
protected:
    ProjectPreferences::PreferencesKey getPreferencesConfigKey() override;

public:
    IotRadioControl(ProjectPreferences &p) : IoTRadio(p) {
        loadCurrentSensors();
    };

    std::string extractGroup(const std::string &signal);
};

#endif
