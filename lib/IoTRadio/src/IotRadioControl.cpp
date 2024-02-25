#include "IotRadioControl.h"

Props IotRadioControl::getPreferencesConfigKey() {
    return IotControl;
}

string IotRadioControl::extractGroup(const string& signal) {
    return signal.substr(0, 3);
}
