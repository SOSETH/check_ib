/*
 * Copyright (C) 2016  Maximilian Falkenstein <mfalkenstein@sos.ethz.ch>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include "IBPort.h"
#include "IBPortRegistry.h"
#include "IBAddress.h"
#include "IBHost.h"

namespace check_ib {

    std::shared_ptr<IBPort> IBPortRegistry::operator[](const uint64_t guid) noexcept {
        return portByGUIDRegistry[guid];
    }

    void IBPortRegistry::registerPort(std::shared_ptr<IBPort> port) noexcept {
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

    std::shared_ptr<IBPort> IBPortRegistry::getPortWithAddress(const IBAddress addr) noexcept {
        return portRegistry[addr];
    }

    bool IBPortRegistry::hasPortWithAddress(const IBAddress addr) const noexcept {
        return portRegistry.count(addr) == 1;
    }

    void IBPortRegistry::registerInterestFor(IBAddress& addr, std::shared_ptr<IBPort> port) throw(IBException) {
        if (portRegistry.count(addr))
            throw IBException("The port you're waiting for already exists!");
        interestRegistry[addr] = port;
    }

    bool IBPortRegistry::isMissingSomething(const bool printOnError) const noexcept {
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
}
