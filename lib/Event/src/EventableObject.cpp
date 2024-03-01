#include "EventableObject.h"

void EventableObject::onEvent(const int& id, const std::function<void(int)>& handler) {
    if (handlers.find(id) != handlers.end()) {
        auto current = handlers.at(id);
        current.push_back(handler);
        handlers.erase(id);
        handlers.insert(make_pair(id, current));
    } else {
        vector<std::function<void(int)>> newHandlers = {handler};
        handlers.insert(make_pair(id, newHandlers));
    }
}

void EventableObject::triggerEvent(const int& id) {
    if (handlers.find(id) != handlers.end()) {
        auto current = handlers.at(id);
        for (const auto &func: current) {
            func(id);
        }
    }
}