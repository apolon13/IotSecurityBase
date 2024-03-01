#include "IoTRadioDetect.h"
#include "IotRadioControl.h"
#include "TaskScheduler.h"
#include "ProjectPreferences.h"
#include "Topic.h"
#include "EventableObject.h"

#ifndef DISPLAY_SECURITY_H
#define DISPLAY_SECURITY_H

enum class SecurityEvent {
    EventOnGuard,
    EventOnDisarm,
    EventOnAlarm,
    EventOnMute
};

class Security: public EventableObject {
protected:
    IoTRadioDetect *ioTRadioDetect;
    IotRadioControl *iotRadioControl;
    ProjectPreferences *projectPreferences;
    Topic *securityCmdTopic;
    Topic *receiveCmdTopic;
    long lastAlarmEvent = 0;

    void listenRadioCommands();

    void listenMqttCommands();

    void handleDetect(const string& signal);

    void handleControl(const string& signal, bool needTriggerEvent = true);

    void guard();

    void disarm();

    void alarm();

    void mute();

public:
    explicit Security(IoTRadioDetect *d, IotRadioControl *c, ProjectPreferences *p, Topic *cmd,
                      Topic *rcv);

    void listen();

    void lockSystem(bool silent);

    void unlockSystem(bool silent);
};

#endif
