
#include "EventableObject.h"

#ifndef DISPLAY_SCREEN_H
#define DISPLAY_SCREEN_H
class Screen: public EventableObject {
public:
    void mutexWrap(const std::function<void()>& cb);
};
#endif
