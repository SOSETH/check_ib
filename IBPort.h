//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBPORT_H
#define CHECK_IB_IBPORT_H


#include <ibnetdisc.h>
#include <ostream>
#include "IBHost.h"
#include "IBException.h"

class IBPortRegistry;

uint32_t getUIntField(MAD_FIELDS fieldA, MAD_FIELDS fieldB, uint8_t *bufA, uint8_t *bufB);
uint32_t getUIntFieldExt(MAD_FIELDS fieldA, MAD_FIELDS fieldB, uint8_t *bufA, uint8_t *bufB);

class IBPort {
public:
    class IBPortException : public virtual IBException {
    public:
        IBPortException(uint64_t myGuid, const std::string &arg) : IBException(arg), guid(myGuid), domain_error(arg) { }
        IBPortException(uint64_t myGuid, const char *arg) : IBException(arg), guid(myGuid), domain_error(arg)  { }
        const char* what() const noexcept;

    private:
        uint64_t guid;
    };

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

    enum PortAttribute {
        SYMBOL_ERRORS,          /* Number of minor link errors on one of the IB lanes of the port */
        LINK_ERRS_RECOVERED,    /* Number of link error recovery by training */
        LINK_DOWN_COUNT,        /* Number of link failuers that couldn't be recovered by training */
        PORT_RX_ERRS,           /* Total number of received packets with error */
        PORT_TX_DISCARDED,      /* Total number of packets that were discarded due to congestion or link error */
        PORT_TX_CONSTR_ERRS,    /* Total number of outbound packets that were dropped due to invalid type or PKEY
                                 * or IP version */
        PORT_RX_CONSTR_ERRS,    /* Like PORT_TX_CONSTR_ERRS but with inbound packets */
        LOCAL_LINK_INTEGR_ERRS, /* Number of times the link error count exceeded the threshold */
        EXCESSIVE_BUFFER_OVERRUNS, /* Number of times an overrun occured in consecutive flow control periods*/
        RX_BYTES,               /* Total number of bytes received */
        TX_BYTES,               /* Total number of bytes transmitted */
        RX_PACKETS,             /* Total number of packets received */
        TX_PACKETS,             /* Total number of packets transmitted */
        TX_WAIT_TIME            /* Number of ticks where port had data to send but couldn't */
    };

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

    static std::shared_ptr<IBPort> make_port(std::shared_ptr<IBHost>, ibnd_port_t*, std::shared_ptr<IBPortRegistry>,
                                             struct ibmad_port *ibmad_port) throw(IBPortException);

    const std::weak_ptr<IBHost> getHost() const {
        return host;
    }

    const std::weak_ptr<IBPort> getPeer() const {
        return peer;
    }

    void setPeer(std::shared_ptr<IBPort> otherPeer) {
        this->peer = otherPeer;
    }

    const uint64_t getGuid() const {
        return guid;
    }

    const uint16_t getLid() const {
        return lid;
    }

    std::map<PortAttribute, uint64_t> getValues() const {
        return values;
    };

    uint64_t operator[](const PortAttribute);

    void resetCounters(ibnd_port_t*, struct ibmad_port *) throw(IBPortException);

    IBAddress getPeerAddr() const {
        return *peerAddress;
    }

    bool hasPeerAddr() const {
        if (peerAddress)
            return true;
        else
            return false;
    }

protected:
    IBPort(std::shared_ptr<IBHost> host, ibnd_port_t* port, struct ibmad_port *ibmad_port) throw(IBPortException);
    PHYSPortState statePhysical;
    LogPortState stateLogical;
    LinkWidth linkWidthActive;
    unsigned int linkWidthSupported, linkWidthEnabled;
    uint64_t guid;
    uint16_t lid;
    unsigned int portNum;
    std::weak_ptr<IBHost> host;
    std::weak_ptr<IBPort> peer;
    std::unique_ptr<IBAddress> peerAddress;
    LinkSpeed linkSpeedActive, linkSpeedEnabled, linkSpeedSupported;
    std::map<PortAttribute, uint64_t> values;

    LinkSpeed getMaxLinkSpeedFromInt(const uint32_t lsInt, const uint32_t fdr10,
                                     const uint32_t eSpeed) const throw(IBPortException);
    LinkWidth getLinkMaxWidthFromInt(const uint32_t lwInt) const throw(IBPortException);
    PHYSPortState getPHYSPortStateFromInt(const uint32_t ppsInt) const throw(IBPortException);
    LogPortState getLogPortStateFromInt(const uint32_t lpsInt) const throw(IBPortException);
    void queryPort(ibnd_port_t*, struct ibmad_port *) throw(IBPortException);

private:
    int numAttrs = 9;
    int numCount = 5;
    struct {
        PortAttribute key;
        MAD_FIELDS value;
        uint32_t (*fun)(MAD_FIELDS, MAD_FIELDS, uint8_t*, uint8_t*);
    } attributeList[9] = {
            // Key in our enum, Key in IB MAD enum, parse function
            {SYMBOL_ERRORS, IB_PC_ERR_SYM_F, &getUIntField},
            {LINK_ERRS_RECOVERED, IB_PC_LINK_RECOVERS_F, &getUIntField},
            {LINK_DOWN_COUNT, IB_PC_LINK_DOWNED_F, &getUIntField},
            {PORT_RX_ERRS, IB_PC_ERR_RCV_F, &getUIntField},
            {PORT_TX_DISCARDED, IB_PC_XMT_DISCARDS_F, &getUIntField},
            {PORT_TX_CONSTR_ERRS, IB_PC_ERR_XMTCONSTR_F, &getUIntField},
            {PORT_RX_CONSTR_ERRS, IB_PC_ERR_RCVCONSTR_F, &getUIntField},
            {LOCAL_LINK_INTEGR_ERRS, IB_PC_ERR_LOCALINTEG_F, &getUIntField},
            {EXCESSIVE_BUFFER_OVERRUNS, IB_PC_ERR_EXCESS_OVR_F, &getUIntField}
    };
    struct {
        PortAttribute key;
        MAD_FIELDS valueA;
        MAD_FIELDS valueB;
        uint32_t (*fun)(MAD_FIELDS, MAD_FIELDS, uint8_t*, uint8_t*);
    } counterList[5] = {
            // Key in our enum, Key in IB MAD enum, Key in IB MAD enum if extended, parse function
            {TX_BYTES, IB_PC_XMT_BYTES_F, IB_PC_EXT_XMT_BYTES_F, &getUIntFieldExt},
            {RX_BYTES, IB_PC_RCV_BYTES_F, IB_PC_EXT_RCV_BYTES_F, &getUIntFieldExt},
            {TX_PACKETS, IB_PC_XMT_PKTS_F, IB_PC_EXT_XMT_PKTS_F, &getUIntFieldExt},
            {RX_PACKETS, IB_PC_RCV_PKTS_F, IB_PC_EXT_RCV_PKTS_F, &getUIntFieldExt},
            {TX_WAIT_TIME, IB_PC_XMT_WAIT_F, IB_PC_XMT_WAIT_F, &getUIntField}
    };

private:
    static std::map<IBPort::PortAttribute, std::string> attributeDesc;

public:
    static std::map<IBPort::PortAttribute, std::string> getAttributeDescriptions() {
        return attributeDesc;
    }
};

std::ostream& operator<<(std::ostream&, const IBPort::PHYSPortState);
std::ostream& operator<<(std::ostream&, const IBPort::LogPortState);
std::ostream& operator<<(std::ostream&, const IBPort::LinkWidth);
std::ostream& operator<<(std::ostream&, const IBPort::LinkSpeed);
std::ostream& operator<<(std::ostream&, const IBPort::PortAttribute);
std::ostream& operator<<(std::ostream&, const IBPort*);

#endif //CHECK_IB_IBPORT_H
