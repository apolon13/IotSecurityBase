#include "IotRadioControl.h"

using namespace std;

Store::Key IotRadioControl::storeKey() {
    return Store::IotControl;
}

string IotRadioControl::extractGroup(const string &signal) {
    return signal.substr(0, 3);
}
