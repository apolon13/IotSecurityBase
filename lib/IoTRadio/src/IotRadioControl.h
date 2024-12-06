#include "IoTRadio.h"
#include "Store.h"

#ifndef DISPLAY_IOTRADIOCONTROL_H
#define DISPLAY_IOTRADIOCONTROL_H

class IotRadioControl : public IoTRadio {
protected:
    Store::Key storeKey() override;

public:
    IotRadioControl(Store &s, TaskScheduler &sc) : IoTRadio(s, sc) {
        loadCurrentSensors();
    };

    std::string extractGroup(const std::string &signal);
};

#endif
