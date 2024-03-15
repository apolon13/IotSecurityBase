#include "Arduino.h"

#ifndef DISPLAY_LOGGER_H
#define DISPLAY_LOGGER_H


class Logger {
protected:
    Stream *stream;
public:
    explicit Logger(Stream &s) : stream(&s) {};

    Logger() : stream(nullptr) {}

    template<typename T>
    void debug(T msg);
};

template<typename T>
void Logger::debug(T msg) {
    if (stream != nullptr) {
        stream->print("DEBUG: ");
        stream->println(msg);
    }
}

#endif
