//
// Created by maxf on 07.06.16.
//

#include <ostream>
#include "IBPort.h"
#include "IBHost.h"

IBHost::IBHost(ibnd_node_t *host, IBPortRegistry & myRegistry) : registry(myRegistry) {
    guid =  mad_get_field(host->info, 0, IB_NODE_SYSTEM_GUID_F);
    numPorts = static_cast<unsigned int>(host->numports);
    for (int i = 0; i <= numPorts; i++) {
        if (host->ports[i])
            ports.push_back(new IBPort(*this, host->ports[i], registry));
    }
}

IBHost::IBHost() {
    guid = 0;
    numPorts = 0;
}

std::ostream& operator<<(std::ostream& stream, const IBHost& host) {
    stream << "----------------------------------------------------------" << std::endl;
    stream << "Host:" << std::endl << std::endl
            << "\tGUID: " << host.getGUID() << std::endl
            << "\tNumber of ports: " << host.getNumPorts() << std::endl;
    for (auto ports = host.getPorts().begin(); ports != host.getPorts().end(); ++ports)
        stream << *(*ports) << std::endl;
    return stream;
}

