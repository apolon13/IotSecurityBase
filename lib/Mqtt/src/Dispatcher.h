#include <WiFi.h>
#include "ProjectPreferences.h"
#include <vector>
#include "Topic.h"

#ifndef DISPLAY_DISPATCHER_H
#define DISPLAY_DISPATCHER_H

class Dispatcher {

protected:
    ProjectPreferences& projectPreferences;
    PubSubDelegate *pubSubDelegate;
    bool cloudConnectionInProcess = false;
    bool networkConnectionInProcess = false;
    vector<Topic *> topics;
    int networkConnectionAttempts = 0;

    void connectToWiFi();

    void connectToMqtt();

public:
    Dispatcher(ProjectPreferences& p, vector<Topic *> t);

    void connectToNetwork();

    void loop();

    bool networkIsConnected();

    bool cloudIsConnected();

    void connectToCloud();

    int getNetworkConnectionAttempts() const;
};

#endif