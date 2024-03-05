#include "ScreenFactory.h"

ScreenFactory::ScreenFactory(ProjectPreferences &p, IoTRadioDetect &i, Dispatcher &d, QueueTask &q,
                             IotRadioControl &ct) {
    mainScreen = new MainScreen(p, i, d);
    detectSensorsScreen = new DetectSensorsScreen(i, ct, q, p);
    controlSensorsScreen = new ControlSensorsScreen(i, ct, q);
    mqttConfigScreen = new MqttConfigScreen(p);
    wifiSettingsScreen = new WifiSettingsScreen(p);
    generalSettingsScreen = new GeneralSettingsScreen(p);
    editDetectSensorScreen = new EditDetectSensorScreen(i, p);
    lockScreen = new LockScreen(p);
}

MainScreen &ScreenFactory::getMainScreen() {
    return *mainScreen;
}

DetectSensorsScreen &ScreenFactory::getDetectSensorsScreen() {
    return *detectSensorsScreen;
}

ControlSensorsScreen &ScreenFactory::getControlSensorsScreen() {
    return *controlSensorsScreen;
}

MqttConfigScreen &ScreenFactory::getMqttConfigScreen() {
    return *mqttConfigScreen;
}

WifiSettingsScreen &ScreenFactory::getWifiSettingsScreen() {
    return *wifiSettingsScreen;
}

GeneralSettingsScreen &ScreenFactory::getGeneralSettingsScreen() {
    return *generalSettingsScreen;
}

EditDetectSensorScreen &ScreenFactory::getEditDetectSensorScreen() {
    return *editDetectSensorScreen;
}

LockScreen &ScreenFactory::getLockScreen() {
    return *lockScreen;
}
