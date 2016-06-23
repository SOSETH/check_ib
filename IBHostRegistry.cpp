//
// Created by maxf on 23.06.16.
//

#include "IBHost.h"
#include "IBHostRegistry.h"

void IBHostRegistry::addIBHost(std::shared_ptr<IBHost> host){
    hostByGUID[host->getGUID()] = host;
    hostByName[host->getName()] = host;
}

std::shared_ptr<IBHost> IBHostRegistry::operator[](const uint64_t guid) {
    return hostByGUID[guid];
}

std::shared_ptr<IBHost> IBHostRegistry::operator[](const std::string& name) {
    return hostByName[name];
}

std::shared_ptr<IBHost> IBHostRegistry::operator[](const std::string&& name) {
    return hostByName[name];
}

bool IBHostRegistry::has(const std::string& name) const {
    return hostByName.count(name) == 1;
}