#include "Screen.h"

void Screen::onEvent(std::function<void(Events)> handler, Events e) {
    auto iterator = handlers.find(e);
    if (iterator != handlers.end()) {
        auto current = handlers.at(e);
        current.push_back(handler);
        handlers.erase(e);
        handlers.insert(make_pair(e, current));
    } else {
        vector<std::function<void(Events)>> newHandlers = {handler};
        handlers.insert(make_pair(e, newHandlers));
    }
}

void Screen::triggerEvent(Events e) {
    auto iterator = handlers.find(e);
    if (iterator != handlers.end()) {
        auto current = handlers.at(e);
        for (const auto &func: current) {
            func(e);
        }
    }
}