#include "Screen.h"
#include "UiMutex.h"

void Screen::mutexWrap(const std::function<void()> &cb) {
    if (UiMutex::take()) {
        cb();
        UiMutex::give();
    }
}
