#include "ProjectPreferences.h"
#include "SdFat.h"
#include <utility>

#ifndef DISPLAY_FILEPROJECTPREFERENCES_H
#define DISPLAY_FILEPROJECTPREFERENCES_H

class FileProjectPreferences : public ProjectPreferences {
protected:
    std::string filename;
    SdFat sdFat;

    Property readPreferencesProperty(const std::string &name, std::string defaultValue) override;

    void writePreferencesProperty(const std::string &name, const std::string &value) override;

public:
    explicit FileProjectPreferences(std::string fn);
};

#endif //DISPLAY_FILEPROJECTPREFERENCES_H
