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
    EventOnMute,
    UnknownEvent
};

class Security : public EventableObject {
protected:
    IoTRadioDetect &ioTRadioDetect;
    IotRadioControl &iotRadioControl;
    ProjectPreferences &projectPreferences;
    Topic &securityCmdTopic;
    Topic &receiveCmdTopic;
    Topic &alarmTopic;
    long lastAlarmEventTime = 0;
    std::string previousCmd;

    void listenRadioCommands();

    void listenMqttCommands();

    void handleDetect(const std::string &signal);

    void handleControl(const std::string &cmd, bool needTriggerEvent = true);

    void guard();

    void disarm();

    void alarm();

    void mute();

public:
    explicit Security(IoTRadioDetect &d, IotRadioControl &c, ProjectPreferences &p, Topic &cmd, Topic &rcv, Topic &al);

    void listen();

    void lockSystem(bool silent);

    void unlockSystem(bool silent);
};

#endif
