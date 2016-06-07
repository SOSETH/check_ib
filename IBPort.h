//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBPORT_H
#define CHECK_IB_IBPORT_H


#include <ibnetdisc.h>
#include <ostream>
#include "IBHost.h"

class IBPortRegistry;

class IBPort {
public:
    enum PHYSPortState {
        NO_STATE_CHANGE = 0,
        SLEEPING = 1,
        POLLING = 2,
        DISABLED = 3,
        PORT_CONFIG_TRAINING = 4,
        LINK_UP = 5,
        LINK_ERROR_RECOVERY = 6,
        PHY_TEST = 7
    };

    enum LogPortState {
        UNKNOWN = 0,
        DOWN = 1,
        INITIALIZING = 2,
        ARMED = 3,
        ACTIVE = 4
    };

    enum LinkWidth {
        LW_1 = 1,
        LW_4 = 2,
        LW_8 = 4,
        LW_12 = 8
    };

    enum LinkSpeed {
        SDR,
        DDR,
        QDR
    };

    unsigned int getErrorsWrongPKey() const {
        return errorsWrongPKey;
    }

    unsigned int getErrorsWrongMKey() const {
        return errorsWrongMKey;
    }

    unsigned int getErrorsWrongQKey() const {
        return errorsWrongQKey;
    }

    unsigned int getErrorsPhy() const {
        return errorsPhy;
    }

    unsigned int getErrorsOverrun() const {
        return errorsOverrun;
    }

    PHYSPortState getStatePhysical() const {
        return statePhysical;
    }

    LogPortState getStateLogical() const {
        return stateLogical;
    }

    unsigned int getLinkWidthSupported() const {
        return linkWidthSupported;
    }

    unsigned int getLinkWidthEnabled() const {
        return linkWidthEnabled;
    }

    LinkWidth getMaxLinkWidthEnabled() const;

    LinkWidth getMaxLinkWidthSupported() const;

    LinkWidth getLinkWidthActive() const {
        return linkWidthActive;
    }

    unsigned int getLinkSpeedEnabled() const {
        return linkSpeedEnabled;
    }

    unsigned int getPortNum() const {
        return portNum;
    }

    LinkSpeed getMaxLinkSpeedEnabled() const;

    LinkSpeed getMaxLinkSpeedSupported() const;

    LinkSpeed getLinkSpeedActive() const {
        return linkSpeedActive;
    }

    IBPort(IBHost& host, ibnd_port_t* port, IBPortRegistry& registry);

    const IBHost &getHost() const {
        return host;
    }

    const IBPort &getPeer() const {
        return *peer;
    }

    void setPeer(IBPort &peer) {
        this->peer = &peer;
    }

    const uint64_t getGuid() const {
        return guid;
    }

protected:
    unsigned int errorsWrongPKey, errorsWrongMKey, errorsWrongQKey;
    unsigned int errorsPhy, errorsOverrun;
    PHYSPortState statePhysical;
    LogPortState stateLogical;
    unsigned int linkWidthSupported, linkWidthEnabled;
    LinkWidth linkWidthActive;
    uint64_t guid;
    unsigned int portNum;
    IBHost host;
    IBPort* peer;
    unsigned int linkSpeedEnabled, linkSpeedSupported;
    LinkSpeed linkSpeedActive;

    LinkSpeed getLinkSpeedFromInt(const uint32_t lsInt) const;
    LinkWidth getLinkWidthFromInt(const uint32_t lwInt) const;
    PHYSPortState getPHYSPortStateFromInt(const uint32_t ppsInt) const;
    LogPortState getLogPortStateFromInt(const uint32_t lpsInt) const;
    IBPort();
};

std::ostream& operator<<(std::ostream&, const IBPort::PHYSPortState);
std::ostream& operator<<(std::ostream&, const IBPort::LogPortState);
std::ostream& operator<<(std::ostream&, const IBPort::LinkWidth);
std::ostream& operator<<(std::ostream&, const IBPort::LinkSpeed);
std::ostream& operator<<(std::ostream&, const IBPort&);

#endif //CHECK_IB_IBPORT_H
