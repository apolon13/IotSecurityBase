#include <WiFi.h>
#include "ProjectPreferences.h"
#include <vector>
#include "TopicsContainer.h"

#ifndef DISPLAY_DISPATCHER_H
#define DISPLAY_DISPATCHER_H

class Dispatcher {

protected:
    ProjectPreferences &projectPreferences;
    PubSubDelegate *pubSubDelegate;
    bool cloudConnectionInProcess = false;
    bool networkConnectionInProcess = false;
    TopicsContainer &topicsContainer;
    int networkConnectionAttempts = 0;

    void connectToWiFi();

    void connectToMqtt();

public:
    Dispatcher(ProjectPreferences &p, TopicsContainer &t);

    void connectToNetwork();

    void loop();

    bool networkIsConnected();

    bool cloudIsConnected();

    void connectToCloud();

    int getNetworkConnectionAttempts() const;
};

#endif