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

#include "IBAddress.h"
#include "IBPort.h"

namespace check_ib {
    IBAddress::IBAddress(ibnd_port_t *port) noexcept {
        guid = port->guid;
        if (port->node->type == IB_NODE_SWITCH)
            lid = port->node->smalid;
        else
            lid = port->base_lid;
        num = port->portnum;
    }

    IBAddress::IBAddress(IBPort &port) noexcept {
        guid = port.getGuid();
        lid = port.getLid();
        num = port.getPortNum();
    }

    bool IBAddress::operator<(const IBAddress &other) const noexcept {
        if (guid == other.guid) {
            if (lid == other.lid) {
                return num < other.num;
            } else {
                return lid < other.lid;
            }
        } else {
            return guid < other.guid;
        }
    }

    std::ostream &operator<<(std::ostream &stream, const IBAddress &value) noexcept {
        return stream << "IBAddress(GUID: " << std::hex << value.getGuid() << std::dec
               << ", LID: " << value.getLid() << ", Port number: " << value.getNum() << ")" << std::endl;
    }
}
