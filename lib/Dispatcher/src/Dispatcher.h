#include <WiFi.h>
#include "ProjectPreferences.h"

#ifndef DISPLAY_DISPATCHER_H
#define DISPLAY_DISPATCHER_H
typedef std::function<void()> onDisconnect;

class Dispatcher {

protected:
    Logger *logger;
    ProjectPreferences *projectPreferences;
    bool needReconnectCloud = false;
    bool cloudConnectionInProcess = false;
    bool networkConnectionInProcess = false;

    void connectToWiFi();

    void connectToMqtt();

public:
    Dispatcher(ProjectPreferences *p, Logger *l);

    void connectToNetwork();

    void reconnectToCloud();

    void loop();

    bool networkIsConnected();

    bool cloudIsConnected();

    bool hasNeedReconnectCloud();

    void connectToCloud();
};
#endif