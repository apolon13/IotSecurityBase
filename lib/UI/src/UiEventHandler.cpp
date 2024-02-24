#include "UiEventHandler.h"

UiEventHandler::UiEventHandler(ProjectPreferences *p, IoTRadioDetect *i, Dispatcher *d, QueueTask *q,
                               IotRadioControl *ct) {
    mainScreen = new MainScreen(p, i, d);
    detectSensorsScreen = new DetectSensorsScreen(i, ct, q, p);
    controlSensorsScreen = new ControlSensorsScreen(i, ct, q);
    mqttConfigScreen = new MqttConfigScreen(p);
    wifiSettingsScreen = new WifiSettingsScreen(p);
    generalSettingsScreen = new GeneralSettingsScreen(p);
    editDetectSensorScreen = new EditDetectSensorScreen(i, p);
    lockScreen = new LockScreen(p);
}

MainScreen *UiEventHandler::getMainScreen() {
    return mainScreen;
}

DetectSensorsScreen *UiEventHandler::getDetectSensorsScreen() {
    return detectSensorsScreen;
}

ControlSensorsScreen *UiEventHandler::getControlSensorsScreen() {
    return controlSensorsScreen;
}

MqttConfigScreen *UiEventHandler::getMqttConfigScreen() {
    return mqttConfigScreen;
}

WifiSettingsScreen *UiEventHandler::getWifiSettingsScreen() {
    return wifiSettingsScreen;
}

GeneralSettingsScreen *UiEventHandler::getGeneralSettingsScreen() {
    return generalSettingsScreen;
}

EditDetectSensorScreen *UiEventHandler::getEditDetectSensorScreen() {
    return editDetectSensorScreen;
}

LockScreen *UiEventHandler::getLockScreen() {
    return lockScreen;
}
