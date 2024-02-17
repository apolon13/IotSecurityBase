#include "Logger.h"

class SerialLogger: public Logger {
public:
    SerialLogger(int speed);
    void debug(string msg);
};
