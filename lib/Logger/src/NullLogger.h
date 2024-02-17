//
// Created by Pankov on 28.01.2024.
//

#ifndef DISPLAY_NULLLOGGER_H
#define DISPLAY_NULLLOGGER_H
#include "Logger.h"

class NullLogger: public Logger {
public:
    NullLogger();
    void debug(string msg);
};

#endif //DISPLAY_NULLLOGGER_H
