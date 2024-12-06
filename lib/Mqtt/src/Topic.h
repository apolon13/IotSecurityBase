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
public:
    Topic(std::string n) : topicName(std::move(n)) {};

    std::string name();

    void addHandler(const std::function<void(std::string payload)> &handler);

    void handleData(const std::string &payload);
};

#endif
