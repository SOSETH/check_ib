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

#ifndef CHECK_IB_IBSUBNETMANAGER_H
#define CHECK_IB_IBSUBNETMANAGER_H

#include <memory>
#include "IBException.h"

namespace check_ib {
    class IBPort;
    class IBPortRegistry;

    class IBSubnetManager {
    public:
        enum SMState {
            NOT_ACTIVE = 0,
            DISCOVERING = 1,
            STANDBY = 2,
            MASTER = 3
        };

    private:
        std::shared_ptr<IBPort> port;
        uint32_t actCounter;
        unsigned int priority;
        SMState state;

    public:
        const std::shared_ptr<IBPort> &getPort() const noexcept {
            return port;
        }

        uint32_t getActCounter() const noexcept {
            return actCounter;
        }

        unsigned int getPriority() const noexcept {
            return priority;
        }

        SMState getState() const noexcept {
            return state;
        }

        class IBSubnetManagerException : public virtual IBException {
        public:
            IBSubnetManagerException(const std::string &arg) noexcept : IBException(arg), domain_error(arg) { }
            IBSubnetManagerException(const char *arg) noexcept : IBException(arg), domain_error(arg) { }
        };

        IBSubnetManager(uint8_t *, std::shared_ptr<IBPortRegistry>);
    };

    std::ostream &operator<<(std::ostream &, const IBSubnetManager::SMState) noexcept;
    std::ostream &operator<<(std::ostream &, const IBSubnetManager &) noexcept;
}

#endif //CHECK_IB_IBSUBNETMANAGER_H
