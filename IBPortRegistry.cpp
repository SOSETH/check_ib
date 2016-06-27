//
// Created by maxf on 07.06.16.
//

#include <iostream>
#include "IBPort.h"
#include "IBPortRegistry.h"

std::shared_ptr<IBPort> IBPortRegistry::operator[](const uint64_t guid) {
    return portByGUIDRegistry[guid];
}

void IBPortRegistry::registerPort(std::shared_ptr<IBPort> port) {
    IBAddress portAddr(*port);
    if (portRegistry.find(portAddr) != portRegistry.end())
        throw std::invalid_argument("The port you're registering already exists!");
    portRegistry.insert(std::pair<IBAddress, std::shared_ptr<IBPort>>(portAddr, port));
    if (interestRegistry.count(portAddr) > 0) {
        interestRegistry[portAddr]->setPeer(port);
        port->setPeer(interestRegistry[portAddr]);
        interestRegistry.erase(portAddr);
    }
    if (portByGUIDRegistry.count(port->getGuid()) == 0) {
        // Switch ports tend to have the same GUID *argh*
        portByGUIDRegistry[port->getGuid()] = port;
    }
}

std::shared_ptr<IBPort> IBPortRegistry::getPortWithAddress(const IBAddress addr) {
    return portRegistry[addr];
}

bool IBPortRegistry::hasPortWithAddress(const IBAddress addr) const {
    return portRegistry.count(addr) == 1;
}

void IBPortRegistry::registerInterestFor(IBAddress addr, std::shared_ptr<IBPort> port) {
    if (portRegistry.count(addr))
        throw std::invalid_argument("The port you're waiting for already exists!");
    interestRegistry[addr] = port;
}

bool IBPortRegistry::isMissingSomething(const bool printOnError) const {
    bool somethingMissing = false;
    for (auto entry = interestRegistry.cbegin(); entry != interestRegistry.cend(); entry++) {
        somethingMissing = true;
        if (printOnError) {
            std::cerr << "Port " << entry->first << " is required by:" << std::endl;
            std::cerr << "\tPort " << std::hex << entry->second->getGuid() << std::dec;
            if (auto host = entry->second->getHost().lock()) {
                std::cerr << " of Host " << host->getName();
            }
            std::cerr << std::endl;
        }
    }
    return somethingMissing;
}







