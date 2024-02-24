#include "Topic.h"

string Topic::getName() {
    return topicName;
}

void Topic::handleData(string payload) {
    for (auto func : handlers) {
        func(payload);
    }
}

void Topic::addHandler(function<void(string payload)> handler) {
    handlers.push_back(handler);
}

void Topic::refreshHandlers() {
    handlers = {};
}

void Topic::publish(const string& payload) {
    client->publish(topicName, payload);
}

void Topic::withPubSub(PubSubDelegate *c) {
    client = c;
}
