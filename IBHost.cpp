//
// Created by maxf on 07.06.16.
//

#include <ostream>
#include "IBPort.h"
#include "IBHost.h"
#include <memory>
#include <iostream>

IBHost::IBHost(std::shared_ptr<IBPortRegistry> myRegistry) : registry(myRegistry) {
    guid = 0;
    numPorts = 0;
}

std::shared_ptr<IBHost> IBHost::make_host(ibnd_node_t *host, std::shared_ptr<IBPortRegistry> myRegistry, struct ibmad_port *ibmad_port)  {
    std::shared_ptr<IBHost> retval(new IBHost(myRegistry));
    retval->guid =  mad_get_field(host->info, 0, IB_NODE_SYSTEM_GUID_F);
    retval->numPorts = static_cast<unsigned int>(host->numports);
    for (int i = 0; i <= retval->numPorts; i++) {
        if (host->ports[i]) {
            retval->ports.push_back(IBPort::make_port(retval, host->ports[i], retval->registry, ibmad_port));
        }
    }
    return retval;
}

std::ostream& operator<<(std::ostream& stream, const IBHost* host) {
    stream << "----------------------------------------------------------" << std::endl;
    stream << "Host:" << std::endl << std::endl
            << "\tGUID: " << host->getGUID() << std::endl
            << "\tNumber of ports: " << host->getNumPorts() << std::endl;
    auto ports = host->getPorts();
    for (auto port = ports.begin(); port != ports.end(); port++)
        stream << "UC: " << port->use_count() << ", Val: " << (*port) << std::endl;
    return stream;
}

