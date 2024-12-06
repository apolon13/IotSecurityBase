#include "ProjectPreferences.h"
#include "ProjectPreferencesMutex.h"
#include <map>

using namespace std;

std::map<int, string> ProjectPreferences::cache = {};

void ProjectPreferences::set(ProjectPreferences::PreferencesKey property, const string &value) {
    if (ProjectPreferencesMutex::take()) {
        writePreferencesProperty(convertProperty(property), value);
        cache.insert_or_assign(property, value);
        ProjectPreferencesMutex::give();
    }
}

string ProjectPreferences::get(ProjectPreferences::PreferencesKey property, string defaultValue) {
    if (ProjectPreferencesMutex::take()) {
        try {
            auto val = cache.at(property);
            ProjectPreferencesMutex::give();
            return val;
        } catch (out_of_range &e) {
            auto value = readPreferencesProperty(convertProperty(property), std::move(defaultValue)).value;
            cache.insert(std::make_pair(property, value));
            ProjectPreferencesMutex::give();
            return value;
        }
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

void ProjectPreferences::setNetworkMode(ProjectPreferences::PreferencesKey mode) {
    if (mode == PreferencesKey::SimNetworkMode) {
        set(ProjectPreferences::SimNetworkMode, ON);
        set(ProjectPreferences::WifiNetworkMode, OFF);
    }

    if (mode == PreferencesKey::WifiNetworkMode) {
        set(ProjectPreferences::SimNetworkMode, OFF);
        set(ProjectPreferences::WifiNetworkMode, ON);
    }
}


