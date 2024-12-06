#include <string>
#include <utility>
#include "ProjectPreferences.h"

#ifndef IOTSECURITYBASE_PREFERENCESFACTORY_H
#define IOTSECURITYBASE_PREFERENCESFACTORY_H


class PreferencesFactory {
    std::string ns;
public:
    explicit PreferencesFactory(std::string ns): ns(std::move(ns)) {};
    std::unique_ptr<ProjectPreferences> createPreferences();
};


#endif
