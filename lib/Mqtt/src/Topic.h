#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include "Arduino.h"
#include "PubSubDelegate.h"

#ifndef DISPLAY_TOPIC_H
#define DISPLAY_TOPIC_H

using namespace std;

class Topic {
private:
    string topicName;
    vector<function<void(string payload)>> handlers;
    PubSubDelegate *client;
public:
    explicit Topic(string n) : topicName(std::move(n)) {};

    string getName();

    void withPubSub(PubSubDelegate *c);

    void addHandler(const function<void(string payload)>& handler);

    void publish(const string& payload);

    void refreshHandlers();

    void handleData(const string& payload);
};

#endif
