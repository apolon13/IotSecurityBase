#include "PreferencesFactory.h"
#include "FileProjectPreferences.h"
#include "FlashProjectPreferences.h"


std::unique_ptr<ProjectPreferences> PreferencesFactory::createPreferences() {
    try {
        return std::make_unique<FileProjectPreferences>(ns);
    } catch (FailedInitialization &e) {
        return std::make_unique<FlashProjectPreferences>(ns);
    }
}
