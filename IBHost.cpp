//
// Created by maxf on 07.06.16.
//

#include <ostream>
#include <iostream>
#include "IBHostRegistry.h"
#include "IBPort.h"

IBHost::IBHost(std::shared_ptr<IBPortRegistry> myRegistry) : registry(myRegistry) {
    guid = 0;
    numPorts = 0;
}

std::shared_ptr<IBHost> IBHost::make_host(ibnd_node_t* host, std::shared_ptr<IBPortRegistry> myRegistry,
                                          struct ibmad_port *ibmad_port, std::shared_ptr<IBNetfileParser> nf,
                                          std::shared_ptr<IBHostRegistry> hostRegistry,
                                          boost::program_options::variables_map& options)  {
    std::shared_ptr<IBHost> retval(new IBHost(myRegistry));
    retval->guid = host->guid;
    retval->numPorts = static_cast<unsigned int>(host->numports);
    retval->name = nf->getNodeName(retval->guid);

    bool clear = false;
    if (options.count("clear") && options.count("check-host")) {
        if (options["check-host"].as<std::string>() == retval->name)
            clear = true;
    }
    for (int i = 0; i <= retval->numPorts; i++) {
        if (host->ports[i]) {
            auto port = IBPort::make_port(retval, host->ports[i], retval->registry, ibmad_port);
            retval->ports.push_back(port);
            if (clear)
                port->resetCounters(host->ports[i], ibmad_port);
        }
    }
    hostRegistry->addIBHost(retval);
    return retval;
}

std::ostream& operator<<(std::ostream& stream, const IBHost* host) {
    stream << "----------------------------------------------------------" << std::endl;
    stream << "Host: " << host->getName() << std::endl
            << "\tGUID: " << std::hex << host->getGUID() << std::dec << std::endl
            << "\tNumber of ports: " << host->getNumPorts() << std::endl;
    auto ports = host->getPorts();
    for (auto port = ports.begin(); port != ports.end(); port++)
        stream << "UC: " << port->use_count() << ", Val: " << (*port) << std::endl;
    return stream;
}

