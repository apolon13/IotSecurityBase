#include "Store.h"
#include "IoTRadioDetect.h"
#include "ui.h"
#include "Screen.h"

#ifndef DISPLAY_MAINSCREEN_H
#define DISPLAY_MAINSCREEN_H

class MainScreen : public Screen {
private:
    Store &store;
    IoTRadioDetect &ioTRadioDetect;
public:
    MainScreen(Store &p, IoTRadioDetect &i) : store(p), ioTRadioDetect(i) {};

    void showSuccessNetworkIcon();

    void showFailedNetworkIcon();

    void successMqttConnection();

    void failedMqttConnection();

    void loadDataInMain(lv_event_t *e);

    void changeConnectionIcons(bool cloudIsConnected, bool networkIsConnected);

    void goTo(bool lockScreenState);
};

#endif
