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

#ifndef CHECK_IB_IBADDRESS_H
#define CHECK_IB_IBADDRESS_H

#include <ibnetdisc.h>
#include <ostream>

namespace check_ib {
    class IBPort;

    class IBAddress {
    private:
        uint64_t guid;
        uint16_t lid;
        int num;

    public:
        uint64_t getGuid() const noexcept {
            return guid;
        }

        uint16_t getLid() const noexcept {
            return lid;
        }

        int getNum() const noexcept {
            return num;
        }

        IBAddress(ibnd_port_t *port) noexcept;

        IBAddress(IBPort &port) noexcept;

        bool operator<(const IBAddress &other) const noexcept;
    };

    std::ostream &operator<<(std::ostream &, const IBAddress &) noexcept;
}

#endif //CHECK_IB_IBADDRESS_H
