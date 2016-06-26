//
// Created by maxf on 26.06.16.
//

#include <iostream>
#include "IBNetworkValidator.h"
#include "IBHostRegistry.h"
#include "IBHost.h"
#include "IBPort.h"

#define TIMEOUT 1000

IBNetworkValidator::IBNetworkValidator(std::shared_ptr<IBNetfileParser> parser, std::shared_ptr<IcingaOutput> output,
                                       struct ibmad_port *ibmad_port) :
        IBValidator(parser, output), ibmad_port(ibmad_port) {}

bool IBNetworkValidator::isValid() throw(IBException) {
    const std::unique_ptr<uint8_t[]> buf(new uint8_t[1024]);
    ib_portid_t portID = {0};

    // Find subnet manager
    if (ib_resolve_smlid_via(&portID, TIMEOUT, ibmad_port))
        throw IBNetworkValidatorException("Coudln't resolve subnet manager");

    if (!smp_query_via(buf.get(), &portID, IB_ATTR_SMINFO, 0, TIMEOUT, ibmad_port))
        throw IBNetworkValidatorException("Coudln't query subnet manager");

    int state = 0;
    uint64_t guid = 0;
    mad_decode_field(buf.get(), IB_SMINFO_STATE_F, &state);
    mad_decode_field(buf.get(), IB_SMINFO_GUID_F, &guid);
    // Should be 3
    std::cout << "Master subnet manager state: " << state << " at GUID: " << std::hex << guid << std::dec << std::endl;

    ib_sa_call_t* mycall = new ib_sa_call_t;
    mycall->method = IB_MAD_METHOD_GET_TABLE;
    mycall->attrid = IB_SA_ATTR_SMINFORECORD;
    mycall->mask = 0; // 0 is a wildcard for input -> get *all* records
    mycall->trid = mad_trid();

    // Note: rcvbuf is actually used for input -> we need to clear it
    memset(buf.get(), 0, 1024);
    // Another note: mycall is also used for output
    uint8_t* retval = sa_rpc_call(ibmad_port, buf.get(), &portID, mycall, TIMEOUT);
    if (!retval)
        throw IBNetworkValidatorException("Coudln't query subnet manager (manual query part)");


    // Since the whole interface isn't documented anywhere, me and my copy of the infiniband standard make the educated
    // guess that recsz (receive size?) is probably the number of entries the table I got has.
    int numberOfManagers = mycall->recsz;

    std::cout << "Number of managers: " << numberOfManagers << std::endl;
    retval += mycall->recsz;
    for (int i = 0; i < numberOfManagers; i++) {
        int state = mad_get_field(retval, 0, IB_SMINFO_STATE_F);
        uint64_t guid = mad_get_field64(retval, 0, IB_SMINFO_GUID_F);
        int prio = mad_get_field(retval, 0, IB_SMINFO_PRIO_F);
        int act = mad_get_field(retval, 0, IB_SMINFO_ACT_F);

        std::cout << "I believe i found: act:" << act << " prio: " <<prio << " state: " << state << std::endl;
        std::cout << "\t (GUID: " << std::hex << guid << std::dec << ")" << std::endl;
        retval += mycall->recsz;
    }

    return true;
}