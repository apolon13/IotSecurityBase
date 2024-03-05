#include "IotRadioControl.h"

ProjectPreferences::PreferencesKey IotRadioControl::getPreferencesConfigKey() {
    return ProjectPreferences::IotControl;
}

string IotRadioControl::extractGroup(const string &signal) {
    return signal.substr(0, 3);
}
