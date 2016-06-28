//
// Created by maxf on 23.06.16.
//

#include "IBHost.h"
#include "IBHostRegistry.h"
#include "IBException.h"

namespace check_ib {
    void IBHostRegistry::addIBHost(std::shared_ptr<IBHost> host) throw(IBException) {
        if (hostByGUID[host->getGUID()])
            throw IBException("Host already exists!");
        if (hostByName[host->getName()])
            throw IBException("Host already exists!");
        hostByGUID[host->getGUID()] = host;
        hostByName[host->getName()] = host;
    }

    std::shared_ptr<IBHost> IBHostRegistry::operator[](const uint64_t guid) noexcept {
        return hostByGUID[guid];
    }

    std::shared_ptr<IBHost> IBHostRegistry::operator[](const std::string &name) noexcept {
        return hostByName[name];
    }

    std::shared_ptr<IBHost> IBHostRegistry::operator[](const std::string &&name) noexcept {
        return hostByName[name];
    }

    bool IBHostRegistry::has(const std::string &name) const noexcept {
        return hostByName.count(name) == 1;
    }
}