#include "TopicsContainer.h"

using namespace std;

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
