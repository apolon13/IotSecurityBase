#include "IoTRadio.h"

#ifndef DISPLAY_IOTRADIODETECT_H
#define DISPLAY_IOTRADIODETECT_H

class IoTRadioDetect : public IoTRadio {
protected:
    Props getPreferencesConfigKey() override;

public:
    IoTRadioDetect(ProjectPreferences *p, Logger *l) : IoTRadio(p, l) {
        loadCurrentSensors();
    };
};

#endif
