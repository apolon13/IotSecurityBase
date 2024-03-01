#include "Screen.h"
#include "UiMutex.h"

void Screen::mutexWrap(const function<void()> &cb) {
    if (UiMutex::take()) {
        cb();
        UiMutex::give();
    }
}
