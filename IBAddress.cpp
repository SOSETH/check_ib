//
// Created by maxf on 22.06.16.
//

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
