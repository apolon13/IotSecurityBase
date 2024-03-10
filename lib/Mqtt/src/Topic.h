#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include "Arduino.h"
#include "PubSubDelegate.h"

#ifndef DISPLAY_TOPIC_H
#define DISPLAY_TOPIC_H

class Topic {
private:
    std::string topicName;
    std::vector<std::function<void(std::string payload)>> handlers;
    PubSubDelegate *client;
public:
    explicit Topic(std::string n) : topicName(std::move(n)) {};

    std::string getName();

    void withPubSub(PubSubDelegate *c);

    void addHandler(const std::function<void(std::string payload)> &handler);

    void publish(const std::string &payload);

    void refreshHandlers();

    void handleData(const std::string &payload);
};

#endif
