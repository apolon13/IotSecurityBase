#include "ProjectPreferences.h"
#include "Preferences.h"

#ifndef DISPLAY_FLASHPROJECTPREFERENCES_H
#define DISPLAY_FLASHPROJECTPREFERENCES_H
class FlashProjectPreferences: public ProjectPreferences {
protected:
    Preferences flash;

    Property readPreferencesProperty(const std::string &name, std::string defaultValue) override;

    void writePreferencesProperty(const std::string &name, const std::string &value) override;

public:
    explicit FlashProjectPreferences(const std::string& nameSpace);
};
#endif
