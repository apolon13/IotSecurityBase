#include "ui.h"
#include "UiMutex.h"
#include "IoTRadioSignal.h"
#include "TaskScheduler.h"
#include "Dispatcher.h"

#ifndef DISPLAY_UIEVENTHANDLER_H
#define DISPLAY_UIEVENTHANDLER_H

class UiEventHandler {
private:
    ProjectPreferences *preferences;
    IoTRadioSignal *ioTRadioSignal;
    TaskScheduler *taskScheduler;
    Dispatcher *dispatcher;
public:
    UiEventHandler(ProjectPreferences *p, IoTRadioSignal *i, TaskScheduler *t, Dispatcher *d);

    void loadGeneralSettings(lv_event_t *e);

    void loadDataInMain(lv_event_t *e);

    void loadAllSensors(lv_event_t *e);

    void loadSensor(lv_event_t *e);

    void loadFiWiSettings(lv_event_t *e);

    void loadMqttSettings(lv_event_t *e);

    void saveMqttSettings(lv_event_t *e);

    void saveWifiSettings(lv_event_t *e);

    void deleteSensor(lv_event_t *e);

    void startScan(lv_event_t *e);

    void stopScan(lv_event_t *e);

    void goToEditSensor(lv_event_t *e);

    void saveSensorSettings(lv_event_t *e);

    void lockSystem(lv_event_t *e);

    void unlockSystem(lv_event_t *e);

    void saveGeneralSettings(lv_event_t *e);

    void enableReceive(lv_event_t *e);

    void handleConnections();
};

#endif //DISPLAY_UIEVENTHANDLER_H
