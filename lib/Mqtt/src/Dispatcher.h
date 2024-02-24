#include <WiFi.h>
#include "ProjectPreferences.h"
#include <vector>
#include "Topic.h"

#ifndef DISPLAY_DISPATCHER_H
#define DISPLAY_DISPATCHER_H

class Dispatcher {

protected:
    Logger *logger;
    ProjectPreferences *projectPreferences;
    bool cloudConnectionInProcess = false;
    bool networkConnectionInProcess = false;
    vector<Topic *> topics;

    void connectToWiFi();

    void connectToMqtt();

public:
    Dispatcher(ProjectPreferences *p, Logger *l, vector<Topic *> t);

    void connectToNetwork();

    void loop();

    bool networkIsConnected();

    bool cloudIsConnected();

    void connectToCloud();
};

#endif