#include "IoTRadioDetect.h"
#include "IotRadioControl.h"
#include "TaskScheduler.h"
#include "ProjectPreferences.h"
#include "Topic.h"
#include "EventableObject.h"
#include "TinyGsmClient7670.h"
#include "PubSub.h"

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
    PubSub pubSub;
    long lastAlarmEventTime = 0;
    std::string previousCmd;
    bool muteMode = false;

    void listenRadioCommands();

    void handleDetect(const std::string &signal);

    void guard();

    void disarm();

    void alarm();

    void mute();

    void restart();

public:
    explicit Security(IoTRadioDetect &d, IotRadioControl &c, ProjectPreferences &p, PubSub &ps);

    void listen();

    void lockSystem(bool silent);

    void unlockSystem(bool silent);

    void runCommand(SecurityCommand command);

    void handleControl(const std::string &cmd, bool needTriggerEvent = true);

};

#endif
