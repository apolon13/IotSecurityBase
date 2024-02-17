#include "IoTRadioSignal.h"

#ifndef DISPLAY_SECURITY_H
#define DISPLAY_SECURITY_H
class Security {
protected:
    IoTRadioSignal *ioTRadioSignal;
public:
    explicit Security(IoTRadioSignal *rs): ioTRadioSignal(rs) {};
    void guard();
    void stopGuard();
};
#endif //DISPLAY_SECURITY_H
