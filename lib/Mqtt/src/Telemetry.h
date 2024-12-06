#include "SimNetwork.h"
#include "Topic.h"
#include "PubSub.h"

#ifndef DISPLAY_TELEMETRYTOPIC_H
#define DISPLAY_TELEMETRYTOPIC_H

class Telemetry {
protected:
    PubSub &pubSub;
public:
    explicit Telemetry(PubSub &ps) : pubSub(ps) {};

    void send();
};

#endif
