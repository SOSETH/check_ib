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

#ifndef CHECK_IB_IBPORT_H
#define CHECK_IB_IBPORT_H


#include <ibnetdisc.h>
#include <ostream>
#include <memory>
#include <map>
#include "IBException.h"
#include "IBAddress.h"

namespace check_ib {
    class IBPortRegistry;
    class IBHost;

    uint32_t getUIntField(MAD_FIELDS fieldA, MAD_FIELDS fieldB, uint8_t *bufA, uint8_t *bufB) noexcept;
    uint32_t getUIntFieldExt(MAD_FIELDS fieldA, MAD_FIELDS fieldB, uint8_t *bufA, uint8_t *bufB) noexcept;

    class IBPort {
    public:
        class IBPortException : public virtual IBException {
        public:
            IBPortException(uint64_t myGuid, const std::string &arg) noexcept : IBException(arg), guid(myGuid),
                                                                                domain_error(arg) { }

            IBPortException(uint64_t myGuid, const char *arg) noexcept : IBException(arg), guid(myGuid),
                                                                         domain_error(arg) { }

            const char *what() const noexcept;

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
            EXCESSIVE_BUFFER_OVERRUNS,  /* Number of times an overrun occured in consecutive flow control periods */
            RX_BYTES,               /* Total number of bytes received */
            TX_BYTES,               /* Total number of bytes transmitted */
            RX_PACKETS,             /* Total number of packets received */
            TX_PACKETS,             /* Total number of packets transmitted */
            TX_WAIT_TIME,           /* Number of ticks where port had data to send but couldn't */
            MGMT_PACKETS_DROPPED    /* Amount of dropped management packets. This indicates severe congestion */
        };

        PHYSPortState getStatePhysical() const noexcept {
            return statePhysical;
        }

        LogPortState getStateLogical() const noexcept {
            return stateLogical;
        }

        unsigned int getLinkWidthSupported() const noexcept {
            return linkWidthSupported;
        }

        LinkWidth getMaxLinkWidthSupported() const {
            return getLinkMaxWidthFromInt(linkWidthSupported);
        }

        unsigned int getLinkWidthEnabled() const noexcept {
            return linkWidthEnabled;
        }

        LinkWidth getMaxLinkWidthEnabled() const {
            return getLinkMaxWidthFromInt(linkWidthEnabled);
        }

        LinkWidth getLinkWidthActive() const noexcept {
            return linkWidthActive;
        }

        LinkSpeed getLinkSpeedEnabled() const noexcept {
            return linkSpeedEnabled;
        }

        LinkSpeed getLinkSpeedSupported() const noexcept {
            return linkSpeedSupported;
        }

        unsigned int getPortNum() const noexcept {
            return portNum;
        }

        LinkSpeed getLinkSpeedActive() const noexcept {
            return linkSpeedActive;
        }

        static std::shared_ptr<IBPort> make_port(std::shared_ptr<IBHost>, ibnd_port_t *,
                                                 std::shared_ptr<IBPortRegistry>,
                                                 struct ibmad_port *ibmad_port);

        const std::weak_ptr<IBHost> getHost() const noexcept {
            return host;
        }

        const std::weak_ptr<IBPort> getPeer() const noexcept {
            return peer;
        }

        void setPeer(std::shared_ptr<IBPort> otherPeer) noexcept {
            this->peer = otherPeer;
        }

        const uint64_t getGuid() const noexcept {
            return guid;
        }

        const uint16_t getLid() const noexcept {
            return lid;
        }

        std::map<PortAttribute, uint64_t> getValues() const noexcept {
            return values;
        };

        uint64_t operator[](const PortAttribute) noexcept;

        void resetCounters(ibnd_port_t *, struct ibmad_port *);

        IBAddress getPeerAddr() const noexcept {
            return *peerAddress;
        }

        bool hasPeerAddr() const noexcept {
            if (peerAddress)
                return true;
            else
                return false;
        }

    protected:
        IBPort(std::shared_ptr<IBHost> host, ibnd_port_t *port, struct ibmad_port *ibmad_port);

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
                                         const uint32_t eSpeed) const;

        LinkWidth getLinkMaxWidthFromInt(const uint32_t lwInt) const;

        PHYSPortState getPHYSPortStateFromInt(const uint32_t ppsInt) const;

        LogPortState getLogPortStateFromInt(const uint32_t lpsInt) const;

        void queryPort(ibnd_port_t *, struct ibmad_port *);

    private:
        int numAttrs = 10;
        int numCount = 5;

        struct {
            PortAttribute key;
            MAD_FIELDS value;

            uint32_t (*fun)(MAD_FIELDS, MAD_FIELDS, uint8_t *, uint8_t *);
        } attributeList[10] = {
                // Key in our enum,         Key in IB MAD enum,     parse function
                {SYMBOL_ERRORS,             IB_PC_ERR_SYM_F,        &getUIntField},
                {LINK_ERRS_RECOVERED,       IB_PC_LINK_RECOVERS_F,  &getUIntField},
                {LINK_DOWN_COUNT,           IB_PC_LINK_DOWNED_F,    &getUIntField},
                {PORT_RX_ERRS,              IB_PC_ERR_RCV_F,        &getUIntField},
                {PORT_TX_DISCARDED,         IB_PC_XMT_DISCARDS_F,   &getUIntField},
                {PORT_TX_CONSTR_ERRS,       IB_PC_ERR_XMTCONSTR_F,  &getUIntField},
                {PORT_RX_CONSTR_ERRS,       IB_PC_ERR_RCVCONSTR_F,  &getUIntField},
                {LOCAL_LINK_INTEGR_ERRS,    IB_PC_ERR_LOCALINTEG_F, &getUIntField},
                {EXCESSIVE_BUFFER_OVERRUNS, IB_PC_ERR_EXCESS_OVR_F, &getUIntField},
                {MGMT_PACKETS_DROPPED,      IB_PC_VL15_DROPPED_F,   &getUIntField}
        };

        struct {
            PortAttribute key;
            MAD_FIELDS valueA;
            MAD_FIELDS valueB;

            uint32_t (*fun)(MAD_FIELDS, MAD_FIELDS, uint8_t *, uint8_t *);
        } counterList[5] = {
                // Key in our enum, Key in IB MAD enum, Key in IB MAD enum if extended, parse function
                {TX_BYTES,     IB_PC_XMT_BYTES_F, IB_PC_EXT_XMT_BYTES_F, &getUIntFieldExt},
                {RX_BYTES,     IB_PC_RCV_BYTES_F, IB_PC_EXT_RCV_BYTES_F, &getUIntFieldExt},
                {TX_PACKETS,   IB_PC_XMT_PKTS_F,  IB_PC_EXT_XMT_PKTS_F,  &getUIntFieldExt},
                {RX_PACKETS,   IB_PC_RCV_PKTS_F,  IB_PC_EXT_RCV_PKTS_F,  &getUIntFieldExt},
                {TX_WAIT_TIME, IB_PC_XMT_WAIT_F,  IB_PC_XMT_WAIT_F,      &getUIntField}
        };

    private:
        static std::map<IBPort::PortAttribute, std::string> attributeDesc;

    public:
        static std::map<IBPort::PortAttribute, std::string> getAttributeDescriptions() {
            return attributeDesc;
        }
    };

    std::ostream &operator<<(std::ostream &, const IBPort::PHYSPortState) noexcept;
    std::ostream &operator<<(std::ostream &, const IBPort::LogPortState) noexcept;
    std::ostream &operator<<(std::ostream &, const IBPort::LinkWidth) noexcept;
    std::ostream &operator<<(std::ostream &, const IBPort::LinkSpeed) noexcept;
    std::ostream &operator<<(std::ostream &, const IBPort::PortAttribute) noexcept;
    std::ostream &operator<<(std::ostream &, const IBPort *);
}
#endif //CHECK_IB_IBPORT_H
