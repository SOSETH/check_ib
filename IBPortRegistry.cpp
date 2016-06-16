//
// Created by maxf on 07.06.16.
//

#include "IBPort.h"
#include "IBPortRegistry.h"


void IBPortRegistry::registerPort(std::shared_ptr<IBPort> port) {
    if (portRegistry.find(port->getGuid()) != portRegistry.end())
        return;
    portRegistry.insert(std::pair<uint64_t, std::shared_ptr<IBPort>>(port->getGuid(), port));
    auto iterator = interestRegistry.find(port->getGuid());
    if (iterator != interestRegistry.end()) {
        auto interestedPorts = iterator->second.begin();
        for (; interestedPorts != iterator->second.end(); ++interestedPorts) {
            (*interestedPorts)->setPeer(port);
        }
        interestRegistry.erase(iterator);
    }
}

std::shared_ptr<IBPort> IBPortRegistry::getPortWithGUID(const uint64_t guid) {
    return portRegistry[guid];
}

bool IBPortRegistry::hasPortWithGUID(const uint64_t guid) const {
    return portRegistry.find(guid) != portRegistry.end();
}

void IBPortRegistry::registerInterestFor(uint64_t guid, std::shared_ptr<IBPort> port) {
    if (interestRegistry.find(guid) != interestRegistry.end())
        return;
    auto list = interestRegistry[guid];
    list.push_back(port);
}







