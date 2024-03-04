#include "IoTRadioDetect.h"
#include "IotRadioControl.h"
#include "QueueTask.h"
#include "SensorScreen.h"
#include "ui.h"

#ifndef DISPLAY_CONTROLSENSORSSCREEN_H
#define DISPLAY_CONTROLSENSORSSCREEN_H

struct ReceivedSignalWithControl : public ReceivedSignal {
    lv_obj_t *controlAction{};
};

class ControlSensorsScreen : public SensorScreen {
private:
    IoTRadioDetect ioTRadioDetect;
    IotRadioControl iotRadioControl;
    QueueTask queueTask;
    lv_obj_t *controlAction;
    ReceivedSignalWithControl *currentSignalWithControl;

    void handleReceiveControl(const uint8_t *incomingData);

    void saveNewControl(void *);

public:
    ControlSensorsScreen(const IoTRadioDetect& d, const IotRadioControl& c, const QueueTask& q);

    void loadAllControls(lv_event_t *e);

    void addControl(lv_event_t *e);

    void addControlAction(lv_event_t *e);

    void deleteControl(lv_event_t *e);
};

#endif
