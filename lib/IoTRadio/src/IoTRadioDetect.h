#include "IoTRadio.h"

#ifndef DISPLAY_IOTRADIODETECT_H
#define DISPLAY_IOTRADIODETECT_H

class IoTRadioDetect : public IoTRadio {
protected:
    Store::Key storeKey() override;

public:
    IoTRadioDetect(Store &s, TaskScheduler &sc) : IoTRadio(s, sc) {
        loadCurrentSensors();
    };
};

#endif
