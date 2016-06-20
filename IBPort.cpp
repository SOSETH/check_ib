//
// Created by maxf on 07.06.16.
//

#include <map>
#include "IBPortRegistry.h"
#include "IBPort.h"

IBPort::IBPort(std::shared_ptr<IBHost> myHost, ibnd_port_t* port) : host(myHost) {
    unsigned int capMask = mad_get_field(port->info, 0, IB_PORT_CAPMASK_F);
    linkWidthEnabled = mad_get_field(port->info, 0, IB_PORT_LINK_WIDTH_ENABLED_F);
    linkWidthActive = getLinkMaxWidthFromInt(mad_get_field(port->info, 0, IB_PORT_LINK_WIDTH_ACTIVE_F));
    linkWidthSupported = mad_get_field(port->info, 0, IB_PORT_LINK_WIDTH_SUPPORTED_F);
    stateLogical = getLogPortStateFromInt(mad_get_field(port->info, 0, IB_PORT_STATE_F));
    statePhysical = getPHYSPortStateFromInt(mad_get_field(port->info, 0, IB_PORT_PHYS_STATE_F));
    errorsWrongMKey = mad_get_field(port->info, 0, IB_PORT_MKEY_F);
    errorsWrongPKey = mad_get_field(port->info, 0, IB_PORT_PKEY_VIOL_F);
    errorsWrongQKey = mad_get_field(port->info, 0, IB_PORT_QKEY_VIOL_F);
    errorsPhy = mad_get_field(port->info, 0, IB_PORT_LOCAL_PHYS_ERR_F);
    errorsOverrun = mad_get_field(port->info, 0, IB_PORT_OVERRUN_ERR_F);
    guid = port->guid;
    portNum = static_cast<unsigned int>(port->portnum);

    unsigned int extSpeed = 0, fdr10 = 0;
    if (capMask & IB_PORT_CAP_HAS_EXT_SPEEDS) {
        // Speed above QDR is considered "extended" and stored in a different struct...
        extSpeed = mad_get_field(port->ext_info, 0, IB_PORT_LINK_SPEED_EXT_ACTIVE_F);
        // ...expect for Mellanox, who do something special...
        fdr10 = mad_get_field(port->ext_info, 0, IB_MLNX_EXT_PORT_LINK_SPEED_ACTIVE_F);
    }
    linkSpeedActive = getMaxLinkSpeedFromInt(mad_get_field(port->info, 0, IB_PORT_LINK_SPEED_ACTIVE_F), fdr10, extSpeed);
    if (capMask & IB_PORT_CAP_HAS_EXT_SPEEDS) {
        extSpeed = mad_get_field(port->ext_info, 0, IB_PORT_LINK_SPEED_EXT_ENABLED_F);
        fdr10 = mad_get_field(port->ext_info, 0, IB_MLNX_EXT_PORT_LINK_SPEED_ENABLED_F);
    }
    linkSpeedEnabled = getMaxLinkSpeedFromInt(mad_get_field(port->info, 0, IB_PORT_LINK_SPEED_ENABLED_F), fdr10, extSpeed);
    if (capMask & IB_PORT_CAP_HAS_EXT_SPEEDS) {
        extSpeed = mad_get_field(port->ext_info, 0, IB_PORT_LINK_SPEED_EXT_SUPPORTED_F);
        fdr10 = mad_get_field(port->ext_info, 0, IB_MLNX_EXT_PORT_LINK_SPEED_SUPPORTED_F);
    }
    linkSpeedSupported = getMaxLinkSpeedFromInt(mad_get_field(port->info, 0, IB_PORT_LINK_SPEED_SUPPORTED_F), fdr10, extSpeed);
}

std::shared_ptr<IBPort> IBPort::make_port(std::shared_ptr<IBHost> myHost, ibnd_port_t* port, std::shared_ptr<IBPortRegistry> registry) {
    std::shared_ptr<IBPort> retval(new IBPort(myHost, port));
    if (port->remoteport) {
        if (registry->hasPortWithGUID(port->remoteport->guid))
            retval->peer = registry->getPortWithGUID(port->remoteport->guid);
        else
            registry->registerInterestFor(port->remoteport->guid, retval);
    }
    registry->registerPort(retval);
    return retval;
}

IBPort::LinkSpeed IBPort::getMaxLinkSpeedFromInt(const uint32_t lsInt, const uint32_t fdr10, const uint32_t eSpeed) const {
    if (eSpeed & 2) {
        return EDR;
    } else if (eSpeed & 1) {
        return FDR;
    } else if (eSpeed == 0) {
        if (fdr10 && FDR10)
            return FDR1;
        else {
            if (lsInt & 4) {
                return QDR;
            } else if (lsInt & 2) {
                return DDR;
            } else if (lsInt & 1) {
                return SDR;
            } else {
                throw "Unknown link speed!";
            }
        }
    } else {
        throw "Unknown link speed!";
    }
}

IBPort::LinkWidth IBPort::getLinkMaxWidthFromInt(const uint32_t lwInt) const {
    if (lwInt & 8)
        return LW_12;
    else if (lwInt & 4)
        return LW_8;
    else if (lwInt & 2)
        return LW_4;
    else if (lwInt & 1)
        return LW_1;
    throw "Unknown link width!";
}

IBPort::PHYSPortState IBPort::getPHYSPortStateFromInt(const uint32_t ppsInt) const {
    if (ppsInt <= 7)
        return static_cast<PHYSPortState>(ppsInt);
    else
        throw "Unknown physical port state";
}

IBPort::LogPortState IBPort::getLogPortStateFromInt(const uint32_t lpsInt) const {
    if (lpsInt <= 4)
        return static_cast<LogPortState>(lpsInt);
    else
        throw "Unknown logical port state";
}

std::ostream& operator<<(std::ostream& stream, const IBPort* port) {
    stream << "Port GUID " << std::hex << port->getGuid() << std::dec << ", port number " << port->getPortNum() << std::endl
            << "\t Link:" << std::endl
            << "\t\tWidth " << port->getLinkWidthActive()
            << " out of " << port->getMaxLinkWidthSupported()
            << "(" << port->getMaxLinkWidthEnabled() << " enabled)" << std::endl
            << "\t\tRate " << port->getLinkSpeedActive()
            << " out of " << port->getLinkSpeedSupported()
            << "(" << port->getLinkSpeedEnabled() << " enabled)";

    stream << std::endl
            << "\t\tPhysical link state: " << port->getStatePhysical() << std::endl
            << "\t\tLogical link state: " << port->getStateLogical() << std::endl
            << "\t Error counters: " << std::endl
            << "\t\tPHY errors: " << port->getErrorsPhy() << std::endl
            << "\t\tOverrun: " << port->getErrorsOverrun() << std::endl
            << "\t\tWrong PKey: " << port->getErrorsWrongPKey() << std::endl
            << "\t\tWrong QKey: " << port->getErrorsWrongQKey() << std::endl
            << "\t\tWrong MKey: " << port->getErrorsWrongMKey() << std::endl;

    if (auto peer = port->getPeer().lock()) {
        stream << "\t Peer port:" << std::endl
                << "\t\t GUID: " << std::hex << peer->getGuid() << std::dec << std::endl;

        if (auto peerHost = peer->getHost().lock())
            stream << "\t\t Host: " << std::hex << peerHost->getGUID() << std::dec << std::endl;
    }

    return stream;
}

#define INSERT_ELEMENT(p) strings[p] = #p
std::ostream& operator<<(std::ostream& stream, const IBPort::PHYSPortState value){
    static std::map<IBPort::PHYSPortState, std::string> strings;
    if (strings.size() == 0){
        INSERT_ELEMENT(IBPort::PHYSPortState::NO_STATE_CHANGE);
        INSERT_ELEMENT(IBPort::PHYSPortState::SLEEPING);
        INSERT_ELEMENT(IBPort::PHYSPortState::POLLING);
        INSERT_ELEMENT(IBPort::PHYSPortState::DISABLED);
        INSERT_ELEMENT(IBPort::PHYSPortState::PORT_CONFIG_TRAINING);
        INSERT_ELEMENT(IBPort::PHYSPortState::LINK_UP);
        INSERT_ELEMENT(IBPort::PHYSPortState::LINK_ERROR_RECOVERY);
        INSERT_ELEMENT(IBPort::PHYSPortState::PHY_TEST);
    }

    return stream << strings[value];
}

std::ostream& operator<<(std::ostream& stream, const IBPort::LogPortState value){
    static std::map<IBPort::LogPortState, std::string> strings;
    if (strings.size() == 0){
        INSERT_ELEMENT(IBPort::LogPortState::UNKNOWN);
        INSERT_ELEMENT(IBPort::LogPortState::DOWN);
        INSERT_ELEMENT(IBPort::LogPortState::INITIALIZING);
        INSERT_ELEMENT(IBPort::LogPortState::ARMED);
        INSERT_ELEMENT(IBPort::LogPortState::ACTIVE);
    }

    return stream << strings[value];
}

std::ostream& operator<<(std::ostream& stream, const IBPort::LinkWidth value){
    static std::map<IBPort::LinkWidth, std::string> strings;
    if (strings.size() == 0){
        INSERT_ELEMENT(IBPort::LinkWidth::LW_1);
        INSERT_ELEMENT(IBPort::LinkWidth::LW_4);
        INSERT_ELEMENT(IBPort::LinkWidth::LW_8);
        INSERT_ELEMENT(IBPort::LinkWidth::LW_12);
    }

    return stream << strings[value];
}

std::ostream& operator<<(std::ostream& stream, const IBPort::LinkSpeed value){
    static std::map<IBPort::LinkSpeed, std::string> strings;
    if (strings.size() == 0){
        INSERT_ELEMENT(IBPort::LinkSpeed::SDR);
        INSERT_ELEMENT(IBPort::LinkSpeed::DDR);
        INSERT_ELEMENT(IBPort::LinkSpeed::QDR);
    }

    return stream << strings[value];
}
#undef INSERT_ELEMENT