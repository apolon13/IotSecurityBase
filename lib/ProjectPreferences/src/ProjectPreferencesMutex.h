#include "Arduino.h"

#ifndef DISPLAY_PROJECTPREFERENCESMUTEX_H
#define DISPLAY_PROJECTPREFERENCESMUTEX_H
class ProjectPreferencesMutex {
protected:
    static SemaphoreHandle_t mutex;
public:
    static bool take();

    static bool give();
};
#endif
