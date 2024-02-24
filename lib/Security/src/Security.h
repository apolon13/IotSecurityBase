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
    Topic *securityTopic;

    void listenRadioCommands();
    void listenMqttCommands();
    void handleDetect(string signal);
    void handleControl(string signal);
    void guard();
    void disarm();
    void alarm();
    void mute();
public:
    explicit Security(IoTRadioDetect *d, IotRadioControl *c, UiControl *ui, ProjectPreferences *p, Topic *t);
    void listen();
};
#endif //DISPLAY_SECURITY_H
