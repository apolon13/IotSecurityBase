#include "IoTRadio.h"
#include "ProjectPreferences.h"

#ifndef DISPLAY_IOTRADIOCONTROL_H
#define DISPLAY_IOTRADIOCONTROL_H

class IotRadioControl : public IoTRadio {
protected:
    ProjectPreferences::PreferencesKey getPreferencesConfigKey() override;

public:
    IotRadioControl(ProjectPreferences &p, TaskScheduler &sc) : IoTRadio(p, sc) {
        loadCurrentSensors();
    };

    std::string extractGroup(const std::string &signal);
};

#endif
