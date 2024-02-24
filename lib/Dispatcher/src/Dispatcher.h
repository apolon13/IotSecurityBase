#include <WiFi.h>
#include "ProjectPreferences.h"
#include <vector>
#include "Topic.h"

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
    vector<Topic *> topics;

    void connectToWiFi();

    void connectToMqtt();

public:
    Dispatcher(ProjectPreferences *p, Logger *l, vector<Topic *> t);

    void connectToNetwork();

    void reconnectToCloud();

    void loop();

    bool networkIsConnected();

    bool cloudIsConnected();

    bool hasNeedReconnectCloud();

    void connectToCloud();
};
#endif