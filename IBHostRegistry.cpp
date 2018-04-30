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
