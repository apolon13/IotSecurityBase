#include "ProjectPreferences.h"
#include "IoTRadioDetect.h"
#include "Dispatcher.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_MAINSCREEN_H
#define DISPLAY_MAINSCREEN_H

class MainScreen : public Screen {
private:
    ProjectPreferences& projectPreferences;
    IoTRadioDetect& ioTRadioDetect;
    Dispatcher& dispatcher;
public:
    MainScreen(ProjectPreferences& p, IoTRadioDetect& i, Dispatcher& d) : projectPreferences(p), ioTRadioDetect(i), dispatcher(d) {};

    void showSuccessNetworkIcon();

    void showFailedNetworkIcon();

    void successMqttConnection();

    void failedMqttConnection();

    void loadDataInMain(lv_event_t *e);

    void handleConnections();

    void goTo(bool useMutex);
};

#endif
