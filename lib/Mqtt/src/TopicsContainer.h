#include <utility>

#include "vector"
#include "Topic.h"

#ifndef DISPLAY_TOPICSCONTAINER_H
#define DISPLAY_TOPICSCONTAINER_H

class TopicsContainer {
protected:
    std::vector<Topic *> topics;
public:
    explicit TopicsContainer(std::vector<Topic *> tp) : topics(std::move(tp)) {};

    void setPublishClient(PubSubDelegate *pubSubDelegate);

    void handleSubscribeData(const std::string& topicName, const std::string& data);

    std::vector<std::string> getTopicNames();
};

#endif //DISPLAY_TOPICSCONTAINER_H
