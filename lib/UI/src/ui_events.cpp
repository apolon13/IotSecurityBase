#include "ui.h"
#include "UiControl.h"

extern UiEventHandler *eventHandler;

void loadGeneralSettings(lv_event_t *e) {
    eventHandler->loadGeneralSettings(e);
}

void loadDataInMain(lv_event_t *e) {
    eventHandler->loadDataInMain(e);
}

void loadAllSensors(lv_event_t *e) {
    eventHandler->loadAllSensors(e);
}

void loadSensor(lv_event_t *e) {
    eventHandler->loadSensor(e);
}

void loadFiWiSettings(lv_event_t *e) {
    eventHandler->loadFiWiSettings(e);
}

void loadMqttSettings(lv_event_t *e) {
    eventHandler->loadMqttSettings(e);
}

void saveMqttSettings(lv_event_t *e) {
    eventHandler->saveMqttSettings(e);
}

void saveWifiSettings(lv_event_t *e) {
    eventHandler->saveWifiSettings(e);
}

void deleteSensor(lv_event_t *e) {
    eventHandler->deleteSensor(e);
}

void startScan(lv_event_t *e) {
    eventHandler->startScan(e);
}

void stopScan(lv_event_t *e) {
    eventHandler->stopScan(e);
}

void goToEditSensor(lv_event_t *e) {
    eventHandler->goToEditSensor(e);
}

void saveSensorSettings(lv_event_t *e) {
    eventHandler->saveSensorSettings(e);
}

void lockSystem(lv_event_t *e) {
    eventHandler->lockSystem(e);
}

void unlockSystem(lv_event_t *e) {
    eventHandler->unlockSystem(e);
}

void saveGeneralSettings(lv_event_t *e) {
    eventHandler->saveGeneralSettings(e);
}

void enableReceive(lv_event_t *e) {
    eventHandler->enableReceive(e);
}
