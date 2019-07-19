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

#ifndef CHECK_IB_IBPORTREGISTRY_H
#define CHECK_IB_IBPORTREGISTRY_H

#include <map>
#include <list>
#include <memory>
#include "IBException.h"

namespace check_ib {
    class IBPort;
    class IBAddress;

    class IBPortRegistry {
    public:
        void registerPort(std::shared_ptr<IBPort>) noexcept;

        std::shared_ptr<IBPort> getPortWithAddress(const IBAddress) noexcept;

        bool hasPortWithAddress(const IBAddress) const noexcept;

        void registerInterestFor(IBAddress& guid, std::shared_ptr<IBPort>);

        bool isMissingSomething(const bool printOnErr) const noexcept;

        std::shared_ptr<IBPort> operator[](const uint64_t) noexcept;

    protected:
        std::map<IBAddress, std::shared_ptr<IBPort>> portRegistry;
        std::map<IBAddress, std::shared_ptr<IBPort>> interestRegistry;
        std::map<uint64_t, std::shared_ptr<IBPort>> portByGUIDRegistry;
    };
}

#endif //CHECK_IB_IBPORTREGISTRY_H
