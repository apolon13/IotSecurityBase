#include "Network.h"
#include "ProjectPreferences.h"

#ifndef DISPLAY_NETWORKFACTORY_H
#define DISPLAY_NETWORKFACTORY_H
class NetworkFactory {
protected:
    ProjectPreferences &projectPreferences;
    Stream &stream;
public:
    NetworkFactory(ProjectPreferences &p, Stream &s): projectPreferences(p), stream(s) {};
    std::unique_ptr<Network> createNetwork();
};
#endif
