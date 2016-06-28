//
// Created by maxf on 27.06.16.
//

#include <infiniband/mad.h>
#include "IBSubnetManager.h"
#include "IBPortRegistry.h"
#include "IBPort.h"
#include "IBHost.h"

namespace check_ib {
    IBSubnetManager::IBSubnetManager(uint8_t *ptr, std::shared_ptr<IBPortRegistry> portReg)
            throw(IBSubnetManagerException) {
        int iState = mad_get_field(ptr + 4, 0, IB_SMINFO_STATE_F);
        switch (iState) {
            case 0:
                state = NOT_ACTIVE;
                break;
            case 1:
                state = DISCOVERING;
                break;
            case 2:
                state = STANDBY;
                break;
            case 3:
                state = MASTER;
                break;
            default:
                throw IBSubnetManagerException("Detected usage of reserved values in field 'state' of SMInfo");
        }

        uint64_t guid = mad_get_field64(ptr + 4, 0, IB_SMINFO_GUID_F);
        port = (*portReg)[guid];
        priority = mad_get_field(ptr + 4, 0, IB_SMINFO_PRIO_F);
        actCounter = mad_get_field(ptr + 4, 0, IB_SMINFO_ACT_F);
    }

    std::ostream &operator<<(std::ostream &stream, const IBSubnetManager::SMState value) noexcept {
        static std::map<IBSubnetManager::SMState, std::string> strings;
        if (strings.size() == 0) {
            strings[IBSubnetManager::SMState::DISCOVERING] = "discovering";
            strings[IBSubnetManager::SMState::MASTER] = "master";
            strings[IBSubnetManager::SMState::NOT_ACTIVE] = "not active";
            strings[IBSubnetManager::SMState::STANDBY] = "standby";
        }

        return stream << strings[value];
    }

    std::ostream &operator<<(std::ostream &output, const IBSubnetManager &ptr) noexcept {
        output << "Subnet manager:" << std::endl
        << "\t State: " << ptr.getState() << std::endl
        << "\t Activity counter: " << ptr.getActCounter() << std::endl
        << "\t Priority: " << ptr.getPriority() << std::endl;
        if (auto host = ptr.getPort()->getHost().lock()) {
            output << "\t Host: " << host->getName() << " on port " << ptr.getPort()->getPortNum() << std::endl;
        } else {
            output << "\t GUID: " << ptr.getPort() << " with LID " << ptr.getPort()->getLid() << std::endl;
        }
        return output;
    }
}
