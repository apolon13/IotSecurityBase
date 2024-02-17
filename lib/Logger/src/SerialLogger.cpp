#include "SerialLogger.h"
#include "Logger.h"

SerialLogger::SerialLogger(int speed) {
    Serial.begin(speed);
}

void SerialLogger::debug(string msg) {
    Serial.print("DEBUG: ");
    Serial.println(msg.c_str());
}
