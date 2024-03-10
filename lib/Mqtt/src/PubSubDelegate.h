#include "PubSubClient.h"
#include "string"

#ifndef DISPLAY_PUBSUBDELEGATE_H
#define DISPLAY_PUBSUBDELEGATE_H

class PubSubDelegate {
private:
    PubSubClient &pubSub;
public:
    explicit PubSubDelegate(PubSubClient &ps) : pubSub(ps) {};

    void publish(const std::string &topic, const std::string &payload);
};

#endif
