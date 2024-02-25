#include "IoTRadioDetect.h"
#include "IotRadioControl.h"
#include "TaskScheduler.h"
#include "ProjectPreferences.h"
#include "UiControl.h"
#include "Topic.h"

#ifndef DISPLAY_SECURITY_H
#define DISPLAY_SECURITY_H

class Security {
protected:
    IoTRadioDetect *ioTRadioDetect;
    IotRadioControl *iotRadioControl;
    ProjectPreferences *projectPreferences;
    UiControl *ui;
    Topic *securityCmdTopic;
    Topic *receiveCmdTopic;
    long lastAlarmEvent = 0;

    void listenRadioCommands();

    void listenMqttCommands();

    void handleDetect(const string& signal);

    void handleControl(const string& signal);

    void guard();

    void disarm();

    void alarm();

    void mute();

public:
    explicit Security(IoTRadioDetect *d, IotRadioControl *c, UiControl *ui, ProjectPreferences *p, Topic *cmd,
                      Topic *rcv);

    void listen();
};

#endif
