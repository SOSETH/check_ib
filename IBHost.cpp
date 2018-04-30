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

#include <ostream>
#include <iostream>
#include "IBHost.h"
#include "IBHostRegistry.h"
#include "IBPort.h"
#include "IBPortRegistry.h"
#include "IBNetfileParser.h"

namespace check_ib {
    IBHost::IBHost(std::shared_ptr<IBPortRegistry> myRegistry) noexcept : registry(myRegistry) {
        guid = 0;
        numPorts = 0;
    }

    std::shared_ptr<IBHost> IBHost::make_host(ibnd_node_t *host, std::shared_ptr<IBPortRegistry> myRegistry,
                                              struct ibmad_port *ibmad_port, std::shared_ptr<IBNetfileParser> nf,
                                              std::shared_ptr<IBHostRegistry> hostRegistry,
                                              boost::program_options::variables_map &options) throw(IBException) {
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

    std::ostream &operator<<(std::ostream &stream, const IBHost *host) noexcept {
        stream << "Host: " << host->getName() << std::endl
            << "\tGUID: " << std::hex << host->getGUID() << std::dec << std::endl
            << "\tNumber of ports: " << host->getNumPorts() << std::endl;
        auto ports = host->getPorts();
        for (auto port = ports.begin(); port != ports.end(); port++)
            stream << "UC: " << port->use_count() << ", Val: " << (*port) << std::endl;
        return stream;
    }
}
