#include "SimNetwork.h"
#include "Topic.h"

#ifndef DISPLAY_TELEMETRYTOPIC_H
#define DISPLAY_TELEMETRYTOPIC_H

class TelemetryTopics {
protected:
    PubSubClient &pubSubClient;
public:
    explicit TelemetryTopics(PubSubClient &ps) : pubSubClient(ps) {};

    void sendTelemetry(SimNetwork &network);
};

#endif
