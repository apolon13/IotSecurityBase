#include "ProjectPreferences.h"
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

ProjectPreferences::ProjectPreferences(Logger *l): logger(l) {
    if (!SDFat.begin()) {
        logger->debug("SD card init error");
    }
    if (!SDFat.exists(FILENAME)) {
        auto prefs = SDFat.open(FILENAME, O_CREAT);
        logger->debug((string)"Create new settings file " + FILENAME);
        prefs.close();
    } else {
        logger->debug(string("Settings file - ") + FILENAME);
    }
}

Property readPreferencesProperty(const string &name, string defaultValue) {
    auto dataProvider = SDFat.open(FILENAME);
    Property p = {defaultValue, 0, false};
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
    return p;
}

void writePreferencesProperty(const string &name, string value) {
    Property currentValue = readPreferencesProperty(name, "");
    string newValue = name + DELIMITER + value + TERMINATOR;
    auto dataProvider = SDFat.open(FILENAME, (O_RDWR | O_CREAT | O_AT_END));
    if (currentValue.exist) {
        dataProvider.seek(currentValue.position);
    } else {
        dataProvider.seek(dataProvider.size());
    }
    dataProvider.print(newValue.c_str());
    dataProvider.close();
}

void ProjectPreferences::set(Props property, string value) {
    writePreferencesProperty(convertProperty(property), value);
}

string ProjectPreferences::get(Props property, string defaultValue) {
    return readPreferencesProperty(convertProperty(property), defaultValue).value;
}
