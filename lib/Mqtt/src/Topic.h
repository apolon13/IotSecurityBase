#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include "Arduino.h"
#include "PubSubClient.h"

#ifndef DISPLAY_TOPIC_H
#define DISPLAY_TOPIC_H

class Topic {
private:
    std::string topicName;
    std::vector<std::function<void(std::string payload)>> handlers;
    PubSubClient &pubSubClient;
public:
    Topic(std::string n, PubSubClient &ps) : topicName(std::move(n)), pubSubClient(ps) {};

    std::string getName();

    void addHandler(const std::function<void(std::string payload)> &handler);

    void publish(const std::string &payload);

    void refreshHandlers();

    void handleData(const std::string &payload);
};

#endif
