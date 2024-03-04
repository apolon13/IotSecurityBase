#include "ui.h"
#include "UiControl.h"

extern ScreenFactory *screenFactory;

void loadDataInMain(lv_event_t *e) {
    screenFactory->getMainScreen()->loadDataInMain(e);
}

void loadAllSensors(lv_event_t *e) {
    screenFactory->getDetectSensorsScreen()->loadAllSensors(e);
}

void loadSensor(lv_event_t *e) {
    screenFactory->getEditDetectSensorScreen()->loadSensor(e);
}

void saveSensorSettings(lv_event_t *e) {
    screenFactory->getEditDetectSensorScreen()->saveSensorSettings(e);
}

void loadFiWiSettings(lv_event_t *e) {
    screenFactory->getWifiSettingsScreen()->loadFiWiSettings(e);
}

void loadMqttSettings(lv_event_t *e) {
    screenFactory->getMqttConfigScreen()->loadMqttSettings(e);
}

void saveMqttSettings(lv_event_t *e) {
    screenFactory->getMqttConfigScreen()->saveMqttSettings(e);
}

void saveWifiSettings(lv_event_t *e) {
    screenFactory->getWifiSettingsScreen()->saveWifiSettings(e);
}

void deleteSensor(lv_event_t *e) {
    screenFactory->getDetectSensorsScreen()->deleteSensor(e);
}

void startScan(lv_event_t *e) {
    screenFactory->getDetectSensorsScreen()->startScan(e);
}

void stopScan(lv_event_t *e) {
    screenFactory->getDetectSensorsScreen()->stopScan(e);
}

void goToEditSensor(lv_event_t *e) {
    screenFactory->getDetectSensorsScreen()->goToEditSensor(e);
}

void lockSystem(lv_event_t *e) {
    screenFactory->getLockScreen()->lockSystem(e);
}

void unlockSystem(lv_event_t *e) {
    screenFactory->getLockScreen()->unlockSystem(e);
}

void saveGeneralSettings(lv_event_t *e) {
    screenFactory->getGeneralSettingsScreen()->saveGeneralSettings(e);
}

void loadGeneralSettings(lv_event_t *e) {
    screenFactory->getGeneralSettingsScreen()->loadGeneralSettings(e);
}

void loadAllControls(lv_event_t *e) {
    screenFactory->getControlSensorsScreen()->loadAllControls(e);
}

void addControl(lv_event_t *e) {
    screenFactory->getControlSensorsScreen()->addControl(e);
}

void addControlAction(lv_event_t *e) {
    screenFactory->getControlSensorsScreen()->addControlAction(e);
}

void deleteControl(lv_event_t *e) {
    screenFactory->getControlSensorsScreen()->deleteControl(e);
}
