//
// Created by maxf on 07.06.16.
//

#include "IBPort.h"
#include "IBPortRegistry.h"


void IBPortRegistry::registerPort(IBPort &port) {
    assert (portRegistry.find(port.getGuid()) == portRegistry.end());
    portRegistry.insert(std::pair<uint64_t, IBPort>(port.getGuid(), port));
    auto iterator = interestRegistry.find(port.getGuid());
    if (iterator != interestRegistry.end()) {
        auto interestedPorts = iterator->second.begin();
        for (; interestedPorts != iterator->second.end(); ++interestedPorts) {
            interestedPorts->setPeer(port);
        }
        interestRegistry.erase(iterator);
    }
}

IBPort &IBPortRegistry::getPortWithGUID(const uint64_t guid) {
    return portRegistry[guid];
}

bool IBPortRegistry::hasPortWithGUID(const uint64_t guid) const {
    return portRegistry.find(guid) != portRegistry.end();
}

void IBPortRegistry::registerInterestFor(uint64_t guid, IBPort & port) {
    assert (interestRegistry.find(guid) == interestRegistry.end());
    auto list = interestRegistry[guid];
    list.push_back(port);
}







