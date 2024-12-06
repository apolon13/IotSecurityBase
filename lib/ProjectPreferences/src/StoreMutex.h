#include "Arduino.h"

#ifndef DISPLAY_STOREMUTEX_H
#define DISPLAY_STOREMUTEX_H
class StoreMutex {
protected:
    static SemaphoreHandle_t mutex;
public:
    static bool take();

    static bool give();
};
#endif
