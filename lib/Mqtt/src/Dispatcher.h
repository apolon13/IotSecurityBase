#include "ProjectPreferences.h"
#include <vector>
#include "TopicsContainer.h"
#include "Network.h"
#include <PubSubClient.h>

#ifndef DISPLAY_DISPATCHER_H
#define DISPLAY_DISPATCHER_H

class Dispatcher {

protected:
    ProjectPreferences &projectPreferences;
    PubSubClient &pubSubClient;
    Network &network;
    bool cloudConnectionInProcess = false;
    bool networkConnectionInProcess = false;
    TopicsContainer &topicsContainer;
    int networkConnectionAttempts = 0;

    void connectToMqtt();

public:
    Dispatcher(ProjectPreferences &p, TopicsContainer &t, Network &n, PubSubClient &psc);

    void connectToNetwork();

    void loop();

    bool networkIsConnected();

    bool cloudIsConnected();

    void connectToCloud();

    int getNetworkConnectionAttempts() const;
};

#endif