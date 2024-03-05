#include "PubSubClient.h"
#include "string"

#ifndef DISPLAY_PUBSUBDELEGATE_H
#define DISPLAY_PUBSUBDELEGATE_H

using namespace std;

class PubSubDelegate {
private:
    PubSubClient &pubSub;
public:
    explicit PubSubDelegate(PubSubClient &ps) : pubSub(ps) {};

    void publish(const string &topic, const string &payload);
};

#endif
