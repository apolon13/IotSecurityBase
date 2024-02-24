#include "Arduino.h"

#ifndef DISPLAY_UIMUTEX_H
#define DISPLAY_UIMUTEX_H

class UiMutex {
public:
    static bool take();

    static bool give();
};

#endif //DISPLAY_UIMUTEX_H
