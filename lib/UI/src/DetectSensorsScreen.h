#include "ui.h"
#include "IoTRadio.h"
#include "IoTRadioDetect.h"
#include "IotRadioControl.h"
#include "SensorScreen.h"
#include "QueueTask.h"

#ifndef DISPLAY_DETECTSENSORSSCREEN_H
#define DISPLAY_DETECTSENSORSSCREEN_H

class DetectSensorsScreen : public SensorScreen {
private:
    ProjectPreferences& projectPreferences;
    IoTRadioDetect& ioTRadioDetect;
    IotRadioControl& iotRadioControl;
    QueueTask& queueTask;
    ReceivedSignal *currentSignal;

    void handleReceiveSensor(const uint8_t *incomingData);

    void saveNewSensor(void *);

public:
    DetectSensorsScreen(IoTRadioDetect& d, IotRadioControl& c, QueueTask& q, ProjectPreferences& p);

    void loadAllSensors(lv_event_t *e);

    void startScan(lv_event_t *e);

    void stopScan(lv_event_t *e);

    void goToEditSensor(lv_event_t *e);

    void deleteSensor(lv_event_t *e);
};

#endif
