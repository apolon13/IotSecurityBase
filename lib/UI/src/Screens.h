#include "ui.h"
#include "UiMutex.h"
#include "IoTRadioDetect.h"
#include "TaskScheduler.h"
#include "QueueTask.h"
#include "IotRadioControl.h"
#include "MainScreen.h"
#include "DetectSensorsScreen.h"
#include "ControlSensorsScreen.h"
#include "MqttConfigScreen.h"
#include "WifiSettingsScreen.h"
#include "GeneralSettingsScreen.h"
#include "EditDetectSensorScreen.h"
#include "LockScreen.h"

#ifndef DISPLAY_UIEVENTHANDLER_H
#define DISPLAY_UIEVENTHANDLER_H

class Screens {
private:
    MainScreen *mainScreen;
    DetectSensorsScreen *detectSensorsScreen;
    ControlSensorsScreen *controlSensorsScreen;
    MqttConfigScreen *mqttConfigScreen;
    WifiSettingsScreen *wifiSettingsScreen;
    GeneralSettingsScreen *generalSettingsScreen;
    EditDetectSensorScreen *editDetectSensorScreen;
    LockScreen *lockScreen;
public:
    Screens(Store &p, IoTRadioDetect &i, QueueTask &q, IotRadioControl &ct);

    MainScreen &main();

    DetectSensorsScreen &detectSensors();

    ControlSensorsScreen &controlSensors();

    MqttConfigScreen &mqttConfig();

    WifiSettingsScreen &wifiSettings();

    GeneralSettingsScreen &generalSettings();

    EditDetectSensorScreen &editDetectSensor();

    LockScreen &lock();
};

#endif
