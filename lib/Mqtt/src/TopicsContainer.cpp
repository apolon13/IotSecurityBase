#include "TopicsContainer.h"

void TopicsContainer::setPublishClient(PubSubDelegate *pubSubDelegate) {
    for (auto topic : topics) {
        topic->withPubSub(pubSubDelegate);
    }
}

vector<string> TopicsContainer::getTopicNames() {
    vector<string> names;
    for (auto topic : topics) {
        names.push_back(topic->getName());
    }
    return names;
}

void TopicsContainer::handleSubscribeData(const string& topicName, const string& data) {
    for (auto topic : topics) {
        if (topic->getName() == topicName) {
            topic->handleData(data);
        }
    }
}
