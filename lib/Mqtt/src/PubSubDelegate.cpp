#include "PubSubDelegate.h"

void PubSubDelegate::publish(const string& topic, const string& payload) {
    pubSub->publish(topic.c_str(), payload.c_str());
}