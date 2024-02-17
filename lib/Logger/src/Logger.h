#include "Arduino.h"

using namespace std;

#ifndef DISPLAY_LOGGER_H
#define DISPLAY_LOGGER_H
class Logger {
public:
    virtual void debug(string msg) = 0;
};

#endif //DISPLAY_LOGGER_H
