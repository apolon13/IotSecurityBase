#include "FlashProjectPreferences.h"

using namespace std;

Property FlashProjectPreferences::readPreferencesProperty(const std::string &name, std::string defaultValue) {
    string value = flash.getString(name.c_str(), defaultValue.c_str()).c_str();
    return {value, 0, flash.isKey(name.c_str())};
}

void FlashProjectPreferences::writePreferencesProperty(const std::string &name, const std::string &value) {
    flash.putString(name.c_str(), value.c_str());
}

FlashProjectPreferences::FlashProjectPreferences(const std::string& nameSpace) {
    flash.begin(nameSpace.c_str(), false);
}
