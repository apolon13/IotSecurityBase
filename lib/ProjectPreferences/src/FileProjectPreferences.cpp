#include "FileProjectPreferences.h"
#include "ProjectPreferencesMutex.h"

#define DELIMITER '='
#define TERMINATOR '\n'

using namespace std;

FileProjectPreferences::FileProjectPreferences(std::string fn): filename(std::move(fn)) {
    if (ProjectPreferencesMutex::take()) {
        if (!sdFat.begin()) {
            //Serial.println("SD card init error");
        }
        if (!sdFat.exists(filename.c_str())) {
            auto file = sdFat.open(filename.c_str(), O_CREAT);
            //Serial.println("Create new settings file");
            file.close();
        }
        ProjectPreferencesMutex::give();
    }
}

void FileProjectPreferences::writePreferencesProperty(const string &name, const string &value) {
    Property currentValue = readPreferencesProperty(name, "");
    if (ProjectPreferencesMutex::take()) {
        string newValue = name + DELIMITER + value + TERMINATOR;
        auto dataProvider = sdFat.open(filename.c_str(), (O_RDWR | O_CREAT | O_AT_END));
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

Property FileProjectPreferences::readPreferencesProperty(const string &name, std::string defaultValue) {
    if (ProjectPreferencesMutex::take()) {
        auto dataProvider = sdFat.open(filename.c_str());
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
