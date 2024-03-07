#include "ProjectPreferences.h"
#include "ProjectPreferencesMutex.h"

#include <utility>
#include "SdFat.h"

SdFat SDFat;

using namespace std;

#define FILENAME "/project.txt"
#define DELIMITER '='
#define TERMINATOR '\n'

struct Property {
    string value;
    size_t position;
    bool exist;
};

unordered_map<int, string> ProjectPreferences::cache = {};

ProjectPreferences::ProjectPreferences() {
    if (ProjectPreferencesMutex::take()) {
        if (!SDFat.begin()) {
            Serial.println("SD card init error");
        }
        if (!SDFat.exists(FILENAME)) {
            auto file = SDFat.open(FILENAME, O_CREAT);
            Serial.println("Create new settings file");
            file.close();
        }
        ProjectPreferencesMutex::give();
    }
}

Property readPreferencesProperty(const string &name, string defaultValue) {
    if (ProjectPreferencesMutex::take()) {
        auto dataProvider = SDFat.open(FILENAME);
        Property p = {std::move(defaultValue), 0, false};
        dataProvider.seek(0);
        while (dataProvider.available()) {
            size_t position = dataProvider.position();
            string line = dataProvider.readStringUntil(TERMINATOR).c_str();
            if (line.find(name) != string::npos) {
                p = {
                        line.substr(line.find(DELIMITER) + 1),
                        position,
                        true
                };
                break;
            }
        }
        dataProvider.close();
        ProjectPreferencesMutex::give();
        return p;
    }
}

void writePreferencesProperty(const string &name, const string &value) {
    Property currentValue = readPreferencesProperty(name, "");
    if (ProjectPreferencesMutex::take()) {
        string newValue = name + DELIMITER + value + TERMINATOR;
        auto dataProvider = SDFat.open(FILENAME, (O_RDWR | O_CREAT | O_AT_END));
        if (currentValue.exist) {
            dataProvider.seek(currentValue.position);
        } else {
            dataProvider.seek(dataProvider.size());
        }
        dataProvider.print(newValue.c_str());
        dataProvider.close();
        ProjectPreferencesMutex::give();
    }
}

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
    set(SystemState, SYSTEM_STATE_LOCK);
}

void ProjectPreferences::unlockSystem() {
    set(SystemState, SYSTEM_STATE_UNLOCK);
}

bool ProjectPreferences::systemIsLocked() {
    return get(SystemState, SYSTEM_STATE_UNLOCK) == SYSTEM_STATE_LOCK;
}

string ProjectPreferences::getSecurityTimeout() {
    return readPreferencesProperty(convertProperty(SecurityTimeout), "30").value;
}

string ProjectPreferences::getConnectionTimeout() {
    return readPreferencesProperty(convertProperty(ConnectionTimeout), "3").value;
}

string ProjectPreferences::getConnectionAttemptsBeforeRestart() {
    return readPreferencesProperty(convertProperty(ConnectionAttemptsBeforeRestart), "10").value;
}
