#include "ui.h"
#include "UiMutex.h"
#include "IoTRadioDetect.h"
#include "TaskScheduler.h"
#include "Dispatcher.h"
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

class UiEventHandler {
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
    UiEventHandler(ProjectPreferences *p, IoTRadioDetect *i, Dispatcher *d, QueueTask *q, IotRadioControl *c);

    MainScreen *getMainScreen();

    DetectSensorsScreen *getDetectSensorsScreen();

    ControlSensorsScreen *getControlSensorsScreen();

    MqttConfigScreen *getMqttConfigScreen();

    WifiSettingsScreen *getWifiSettingsScreen();

    GeneralSettingsScreen *getGeneralSettingsScreen();

    EditDetectSensorScreen *getEditDetectSensorScreen();

    LockScreen *getLockScreen();
};

#endif //DISPLAY_UIEVENTHANDLER_H
