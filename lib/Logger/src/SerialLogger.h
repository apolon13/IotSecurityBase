#include "Logger.h"

class SerialLogger : public Logger {
public:
    explicit SerialLogger(int speed);

    void debug(string msg) override;
};
