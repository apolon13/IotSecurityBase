#include "ui.h"
#include "UiControl.h"

extern UiEventHandler *eventHandler;

void loadDataInMain(lv_event_t *e) {
    eventHandler->getMainScreen()->loadDataInMain(e);
}

void loadAllSensors(lv_event_t *e) {
    eventHandler->getDetectSensorsScreen()->loadAllSensors(e);
}

void loadSensor(lv_event_t *e) {
    eventHandler->getEditDetectSensorScreen()->loadSensor(e);
}

void saveSensorSettings(lv_event_t *e) {
    eventHandler->getEditDetectSensorScreen()->saveSensorSettings(e);
}

void loadFiWiSettings(lv_event_t *e) {
    eventHandler->getWifiSettingsScreen()->loadFiWiSettings(e);
}

void loadMqttSettings(lv_event_t *e) {
    eventHandler->getMqttConfigScreen()->loadMqttSettings(e);
}

void saveMqttSettings(lv_event_t *e) {
    eventHandler->getMqttConfigScreen()->saveMqttSettings(e);
}

void saveWifiSettings(lv_event_t *e) {
    eventHandler->getWifiSettingsScreen()->saveWifiSettings(e);
}

void deleteSensor(lv_event_t *e) {
    eventHandler->getDetectSensorsScreen()->deleteSensor(e);
}

void startScan(lv_event_t *e) {
    eventHandler->getDetectSensorsScreen()->startScan(e);
}

void stopScan(lv_event_t *e) {
    eventHandler->getDetectSensorsScreen()->stopScan(e);
}

void goToEditSensor(lv_event_t *e) {
    eventHandler->getDetectSensorsScreen()->goToEditSensor(e);
}

void lockSystem(lv_event_t *e) {
    eventHandler->getLockScreen()->lockSystem(e);
}

void unlockSystem(lv_event_t *e) {
    eventHandler->getLockScreen()->unlockSystem(e);
}

void saveGeneralSettings(lv_event_t *e) {
    eventHandler->getGeneralSettingsScreen()->saveGeneralSettings(e);
}

void loadGeneralSettings(lv_event_t *e) {
    eventHandler->getGeneralSettingsScreen()->loadGeneralSettings(e);
}

void loadAllControls(lv_event_t *e) {
    eventHandler->getControlSensorsScreen()->loadAllControls(e);
}

void addControl(lv_event_t *e) {
    eventHandler->getControlSensorsScreen()->addControl(e);
}

void addControlAction(lv_event_t *e) {
    eventHandler->getControlSensorsScreen()->addControlAction(e);
}

void deleteControl(lv_event_t *e) {
    eventHandler->getControlSensorsScreen()->deleteControl(e);
}
