#include "ui.h"
#include "UiControl.h"
#include "Screens.h"

extern Screens *screens;

void loadDataInMain(lv_event_t *e) {
    screens->main().loadDataInMain(e);
}

void loadAllSensors(lv_event_t *e) {
    screens->detectSensors().loadAllSensors(e);
}

void loadSensor(lv_event_t *e) {
    screens->editDetectSensor().loadSensor(e);
}

void saveSensorSettings(lv_event_t *e) {
    screens->editDetectSensor().saveSensorSettings(e);
}

void loadFiWiSettings(lv_event_t *e) {
    screens->wifiSettings().loadFiWiSettings(e);
}

void loadMqttSettings(lv_event_t *e) {
    screens->mqttConfig().loadMqttSettings(e);
}

void saveMqttSettings(lv_event_t *e) {
    screens->mqttConfig().saveMqttSettings(e);
}

void saveWifiSettings(lv_event_t *e) {
    screens->wifiSettings().saveWifiSettings(e);
}

void deleteSensor(lv_event_t *e) {
    screens->detectSensors().deleteSensor(e);
}

void startScan(lv_event_t *e) {
    screens->detectSensors().startScan(e);
}

void stopScan(lv_event_t *e) {
    screens->detectSensors().stopScan(e);
}

void goToEditSensor(lv_event_t *e) {
    screens->detectSensors().goToEditSensor(e);
}

void lockSystem(lv_event_t *e) {
    screens->lock().lockSystem(e);
}

void unlockSystem(lv_event_t *e) {
    screens->lock().unlockSystem(e);
}

void saveGeneralSettings(lv_event_t *e) {
    screens->generalSettings().saveGeneralSettings(e);
}

void loadGeneralSettings(lv_event_t *e) {
    screens->generalSettings().loadGeneralSettings(e);
}

void loadAllControls(lv_event_t *e) {
    screens->controlSensors().loadAllControls(e);
}

void addControl(lv_event_t *e) {
    screens->controlSensors().addControl(e);
}

void addControlAction(lv_event_t *e) {
    screens->controlSensors().addControlAction(e);
}

void deleteControl(lv_event_t *e) {
    screens->controlSensors().deleteControl(e);
}

void clearSensorsScreen(lv_event_t * e)
{
    screens->controlSensors().clear();
}

void clearControlsScreen(lv_event_t * e)
{
    screens->detectSensors().clear();
}
