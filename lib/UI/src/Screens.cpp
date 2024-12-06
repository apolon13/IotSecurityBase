#include "Screens.h"

Screens::Screens(Store &p, IoTRadioDetect &i, QueueTask &q, IotRadioControl &ct) {
    mainScreen = new MainScreen(p, i);
    detectSensorsScreen = new DetectSensorsScreen(i, ct, q, p);
    controlSensorsScreen = new ControlSensorsScreen(i, ct, q);
    mqttConfigScreen = new MqttConfigScreen(p);
    wifiSettingsScreen = new WifiSettingsScreen(p);
    generalSettingsScreen = new GeneralSettingsScreen(p);
    editDetectSensorScreen = new EditDetectSensorScreen(i, p);
    lockScreen = new LockScreen(p);
}

MainScreen &Screens::main() {
    return *mainScreen;
}

DetectSensorsScreen &Screens::detectSensors() {
    return *detectSensorsScreen;
}

ControlSensorsScreen &Screens::controlSensors() {
    return *controlSensorsScreen;
}

MqttConfigScreen &Screens::mqttConfig() {
    return *mqttConfigScreen;
}

WifiSettingsScreen &Screens::wifiSettings() {
    return *wifiSettingsScreen;
}

GeneralSettingsScreen &Screens::generalSettings() {
    return *generalSettingsScreen;
}

EditDetectSensorScreen &Screens::editDetectSensor() {
    return *editDetectSensorScreen;
}

LockScreen &Screens::lock() {
    return *lockScreen;
}
