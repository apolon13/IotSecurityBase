#include "ProjectPreferences.h"
#include "ProjectPreferencesMutex.h"

#include <utility>

using namespace std;

unordered_map<int, string> ProjectPreferences::cache = {};

void ProjectPreferences::set(ProjectPreferences::PreferencesKey property, const string &value) {
    writePreferencesProperty(convertProperty(property), value);
    cache.insert_or_assign(property, value);
}

string ProjectPreferences::get(ProjectPreferences::PreferencesKey property, string defaultValue) {
    try {
        return cache.at(property);
    } catch (out_of_range &e) {
        auto value = readPreferencesProperty(convertProperty(property), std::move(defaultValue)).value;
        cache.insert_or_assign(property, value);
        return value;
    }
}

void ProjectPreferences::lockSystem() {
    set(SystemState, ON);
}

void ProjectPreferences::unlockSystem() {
    set(SystemState, OFF);
}

bool ProjectPreferences::systemIsLocked() {
    return get(SystemState, OFF) == ON;
}

string ProjectPreferences::getSecurityTimeout() {
    return get(SecurityTimeout, "30");
}

string ProjectPreferences::getConnectionTimeout() {
    return get(ConnectionTimeout, "3");
}

string ProjectPreferences::getConnectionAttemptsBeforeRestart() {
    return get(ConnectionAttemptsBeforeRestart, "10");
}

bool ProjectPreferences::networkModeIsWifi() {
    return !networkModeIsSim() && get(WifiNetworkMode, "1") == "1";
}

bool ProjectPreferences::networkModeIsSim() {
    return get(SimNetworkMode, "0") == "1";
}
