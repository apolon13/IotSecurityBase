#include "IoTRadioDetect.h"
#include "IotRadioControl.h"
#include "TaskScheduler.h"
#include "ProjectPreferences.h"
#include "Topic.h"
#include "EventableObject.h"
#include "TinyGsmClient7670.h"

#ifndef DISPLAY_SECURITY_H
#define DISPLAY_SECURITY_H

enum class SecurityEvent {
    EventOnGuard,
    EventOnDisarm,
    EventOnAlarm,
    EventOnMute,
    EventOnRestart,
    UnknownEvent
};

enum class SecurityCommand {
    Guard,
    Disarm,
    Alarm,
    Restart,
    Mute
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
    int lastSmsIndex = 1;

    void listenRadioCommands();

    void listenMqttCommands();

    void handleDetect(const std::string &signal);

    void handleControl(const std::string &cmd, bool needTriggerEvent = true);

    void guard();

    void disarm();

    void alarm();

    void mute();

    void restart();

public:
    explicit Security(IoTRadioDetect &d, IotRadioControl &c, ProjectPreferences &p, Topic &cmd, Topic &rcv, Topic &al);

    void listen();

    void listenSmsCommands(TinyGsmSim7670 &modem);

    void lockSystem(bool silent);

    void unlockSystem(bool silent);

    void runCommand(SecurityCommand command);
};

#endif
