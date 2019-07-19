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

#ifndef CHECK_IB_IBHOSTREGISTRY_H
#define CHECK_IB_IBHOSTREGISTRY_H

#include <map>
#include <memory>
#include "IBException.h"

namespace check_ib {
    class IBHost;

    class IBHostRegistry {
    private:
        std::map<uint64_t, std::shared_ptr<IBHost>> hostByGUID;
        std::map<std::string, std::shared_ptr<IBHost>> hostByName;

    public:
        void addIBHost(std::shared_ptr<IBHost>);

        std::shared_ptr<IBHost> operator[](const uint64_t) noexcept;

        std::shared_ptr<IBHost> operator[](const std::string &) noexcept;

        std::shared_ptr<IBHost> operator[](const std::string &&) noexcept;

        bool has(const std::string &) const noexcept;

        const std::map<std::string, std::shared_ptr<IBHost>> &getAllByName() const noexcept {
            return hostByName;
        };
    };
}

#endif //CHECK_IB_IBHOSTREGISTRY_H
