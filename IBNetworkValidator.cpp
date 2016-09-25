//
// Created by maxf on 26.06.16.
//

#include <iostream>
#include <iomanip>
#include "IBNetworkValidator.h"
#include "IBHostRegistry.h"
#include "IBPortRegistry.h"
#include "IBHost.h"
#include "IBPort.h"
#include "IBSubnetManager.h"
#include "IcingaOutput.h"
#include "IBNetfileParser.h"

namespace check_ib {

#define TIMEOUT 100

    IBNetworkValidator::IBNetworkValidator(struct ibmad_port *ibmad_port,
                                           boost::program_options::variables_map &options,
                                           std::shared_ptr<IBPortRegistry> portRegistry)
            noexcept : IBValidator(), ibmad_port(ibmad_port), options(options), portRegistry(portRegistry) { }

    bool IBNetworkValidator::isValid(std::shared_ptr<IBNetfileParser> parser, std::shared_ptr<IcingaOutput> output,
                                     std::shared_ptr<IBHostRegistry> hostRegistry) throw(IBException) {
        const std::unique_ptr<uint8_t[]> buf(new uint8_t[1024]);
        ib_portid_t portID = {0};

        /* * * * * * * * * Part 1: Find the master subnet manager * * * * * * * * */
        if (ib_resolve_smlid_via(&portID, TIMEOUT, ibmad_port))
            throw IBNetworkValidatorException("Coudln't resolve subnet manager");

        /* * * * * * * * * Part 2: Find all subnet managers * * * * * * * * */
        ib_sa_call_t *mycall = new ib_sa_call_t;
        mycall->method = IB_MAD_METHOD_GET_TABLE;
        mycall->attrid = IB_SA_ATTR_SMINFORECORD;
        mycall->mask = 0; // 0 is a wildcard for input -> get *all* records
        mycall->trid = mad_trid();

        // Note: rcvbuf is actually also used for input -> make sure to always clear it
        memset(buf.get(), 0, 1024);
        // Another note: mycall is also used for output
        uint8_t *retval = sa_rpc_call(ibmad_port, buf.get(), &portID, mycall, TIMEOUT);
        if (!retval)
            throw IBNetworkValidatorException("Coudln't query subnet manager (manual query part)");

        /* Today's favorite in *lemme-guess-what-this-pointer-thingy does*: What does sa_rpc_call return?
         * Since I couldn't find any documentation on this (surprise) I resorted to hexdumping stuff:
         * 6 SMs:
         *   0 | 00 08 00 00 00 08 f1 04 03 99 12 4d 00 00 00 00 00 00 00 00 00 00 07 d4 03 00 00 00 00 00 00 00
         *        LID | res |         GUID          | SMKey                 | Activity c|st| Padding
         * 031 | 00 01 00 00 00 08 f1 04 03 99 14 29 00 00 00 00 00 00 00 00 00 06 6e 8a 02 00 00 00 00 00 00 00
         * 063 | 00 04 00 00 00 08 f1 04 03 99 14 2a 00 00 00 00 00 00 00 00 00 05 81 9b 02 00 00 00 00 00 00 00
         * 095 | 00 05 00 00 00 08 f1 04 03 99 12 4e 00 00 00 00 00 00 00 00 00 00 02 bb 02 00 00 00 00 00 00 00
         * 127 | 00 02 00 00 00 08 f1 04 03 99 41 a9 00 00 00 00 00 00 00 00 00 01 c1 29 02 00 00 00 00 00 00 00
         * 159 | 00 0e 00 00 00 08 f1 04 03 99 41 aa 00 00 00 00 00 00 00 00 00 01 c1 12 02 00 00 00 00 00 00 00
         * 191 | 6c 2b db c5 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
         * 3 SMs:
         *   0 | 00 03 00 00 00 08 f1 04 03 9a 34 12 00 00 00 00 00 00 00 00 00 01 74 3b 03 00 00 00 00 00 00 00
         *        LID | res |         GUID          | SMKey                 | Activity c|st| Padding
         * 031 | 00 02 00 00 00 08 f1 04 03 9a 36 09 00 00 00 00 00 00 00 00 00 00 ba d0 02 00 00 00 00 00 00 00
         * 063 | 00 01 00 00 00 08 f1 04 03 9a 35 5d 00 00 00 00 00 00 00 00 00 01 18 1d 02 00 00 00 00 00 00 00
         * 095 | 28 66 a2 00 00 00 00 00 80 8c c2 48 ff 7f 00 00 20 66 a2 00 09 00 00 00 90 8c c2 48 ff 7f 00 00
         * 127 | cd c7 49 00 00 00 00 00 b0 8c c2 48 ff 7f 00 00 02 00 00 00 00 00 00 00 28 8c a3 00 00 00 00 00
         * 159 | 68 e8 a2 00 00 00 00 00 d0 8c c2 48 ff 7f 00 00 1c dc ba da 8b 7f 00 00 20 8f c2 48 ff 7f 00 00
         * 191 | 6c 2b db c5 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
         * where res is "reserved" as per infiniband specification and st is the status/priority field (4 bit each).
         * Each record looks exactly like it should (as per infiniband specification), the question is how are they aligned?
         * My copy of the IB spec tells me that an SMInfo record is 200 Bits (25 Bytes), in this case, its 32 Bytes
         * Probably alignment or something. It remains to find out how many records there are. Since "recsz" contains
         * - no, not how big the packet is, that'd be too easy, right? - the offset where data starts from the beginning
         * of the packet as needed for mad_get_field(4 bytes in this case), we need to manually find out how big our packet
         * is. We assume that the padding for each valid row is zero
         */
        int size = 1024;
        while (size >= 0 && retval[size - 1] == 0) {
            size--;
        }
        if (size < 38)
            throw IBNetworkValidatorException("Implausible packet size!");
        int numberOfManagers = 0;
        for (uint8_t* row=retval ; row<retval+size; row+=32) {
            // Padding starts at row+25 bytes and is 7 byte long
            if ((*((uint32_t*)row+25)) == 0 && (*(row+29))==0 && (*(row+30))==0 && (*(row+31))==0)
                numberOfManagers++;
            else
                break;
        }


        if (options.count("dump")) {
            std::cout << "Packet size: " << size << std::endl;
            dumpBuffer(retval , 1020);
            std::cout << "Number of managers from pointer oracle: " << numberOfManagers << std::endl;
        }

        /* * * * * * * * * Part 3: Construct objects * * * * * * * * */
        std::shared_ptr<std::map<uint64_t, std::shared_ptr<IBSubnetManager>>> smMap(
                new std::map<uint64_t, std::shared_ptr<IBSubnetManager>>);
        for (int i = 0; i < numberOfManagers; i++) {
            auto sm = new IBSubnetManager(retval, portRegistry);
            (*smMap)[sm->getPort()->getGuid()] = std::shared_ptr<IBSubnetManager>(sm);
            retval += 32;
        }
        if (options.count("dump")) {
            for (auto sm = smMap->begin(); sm != smMap->end(); sm++) {
                std::cout << *(sm->second);
            }
        }
        output->setIBSubnetManagersDetail(smMap);

        /* * * * * * * * * Part 4: Get to work * * * * * * * * */
        bool isOk = true;
        std::map<uint64_t, std::shared_ptr<IBSubnetManager>> workMap;
        workMap.insert(smMap->begin(), smMap->end());
        std::list<uint64_t> registeredSMs = parser->getSubnetManagers();
        for (auto smInt = registeredSMs.begin(); smInt != registeredSMs.end(); smInt++) {
            auto smHost = (*hostRegistry)[*smInt];
            if (!smHost) {
                output->failWarning() << "There's a subnet manager on a host we don't know! GUID: " << std::hex << *smInt << std::dec;
                isOk = false;
                continue;
            }
            auto ports = smHost->getPorts();
            for (auto port = ports.begin(); port != ports.end(); port++) {
                if (workMap.count((*port)->getGuid()) == 0) {
                    output->failWarning() << "Missing subnet manager at " << (*port);
                    isOk = false;
                } else {
                    workMap.erase((*port)->getGuid());
                }
            }
        }
        if (workMap.size() > 0) {
            output->failWarning() << "More subnet managers than expected!";
            isOk = false;
        }
        output->printPerformanceData(smMap);
        output->finish();

        return isOk;
    }

    void IBNetworkValidator::dumpBuffer(uint8_t *buf, int len) {
        std::cout << "  0 | ";
        for (int i = 0; i < len; ++i) {
            std::cout << std::setfill('0') << std::setw(2) << std::hex << (int) (buf[i]) << std::dec << " ";
            if (i > 0 && (i + 1) % 32 == 0 && i < (len - 1)) {
                std::cout << std::endl;
                std::cout << std::setw(3) << i << " | ";
            }
        }
        std::cout << std::endl;
    }
}