//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBPORT_H
#define CHECK_IB_IBPORT_H


#include <ibnetdisc.h>
#include <ostream>
#include "IBHost.h"

class IBPortRegistry;

class IBPort : public std::enable_shared_from_this<IBPort> {
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
        SDR = 1,
        DDR = 2,
        QDR = 4,
        FDR1 = 8,
        FDR = 16,
        EDR = 32
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

    LinkWidth getMaxLinkWidthSupported() const {
        return getLinkMaxWidthFromInt(linkWidthSupported);
    }

    unsigned int getLinkWidthEnabled() const {
        return linkWidthEnabled;
    }

    LinkWidth getMaxLinkWidthEnabled() const {
        return getLinkMaxWidthFromInt(linkWidthEnabled);
    }

    LinkWidth getLinkWidthActive() const {
        return linkWidthActive;
    }

    LinkSpeed getLinkSpeedEnabled() const {
        return linkSpeedEnabled;
    }

    LinkSpeed getLinkSpeedSupported() const {
        return linkSpeedSupported;
    }

    unsigned int getPortNum() const {
        return portNum;
    }

    LinkSpeed getLinkSpeedActive() const {
        return linkSpeedActive;
    }

    static std::shared_ptr<IBPort> make_port(std::shared_ptr<IBHost>, ibnd_port_t*, std::shared_ptr<IBPortRegistry>);

    const std::weak_ptr<IBHost> getHost() const {
        return host;
    }

    const std::weak_ptr<IBPort> getPeer() const {
        return peer;
    }

    void setPeer(std::shared_ptr<IBPort> peer) {
        this->peer = peer;
    }

    const uint64_t getGuid() const {
        return guid;
    }

protected:
    IBPort(std::shared_ptr<IBHost> host, ibnd_port_t* port);
    unsigned int errorsWrongPKey, errorsWrongMKey, errorsWrongQKey;
    unsigned int errorsPhy, errorsOverrun;
    PHYSPortState statePhysical;
    LogPortState stateLogical;
    LinkWidth linkWidthActive;
    unsigned int linkWidthSupported, linkWidthEnabled;
    uint64_t guid;
    unsigned int portNum;
    std::weak_ptr<IBHost> host;
    std::weak_ptr<IBPort> peer;
    LinkSpeed linkSpeedActive, linkSpeedEnabled, linkSpeedSupported;

    LinkSpeed getMaxLinkSpeedFromInt(const uint32_t lsInt, const uint32_t fdr10, const uint32_t eSpeed) const;
    LinkWidth getLinkMaxWidthFromInt(const uint32_t lwInt) const;
    PHYSPortState getPHYSPortStateFromInt(const uint32_t ppsInt) const;
    LogPortState getLogPortStateFromInt(const uint32_t lpsInt) const;
};

std::ostream& operator<<(std::ostream&, const IBPort::PHYSPortState);
std::ostream& operator<<(std::ostream&, const IBPort::LogPortState);
std::ostream& operator<<(std::ostream&, const IBPort::LinkWidth);
std::ostream& operator<<(std::ostream&, const IBPort::LinkSpeed);
std::ostream& operator<<(std::ostream&, const IBPort*);

#endif //CHECK_IB_IBPORT_H
