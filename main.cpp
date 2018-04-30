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

#include <iostream>
#include <list>
#include <infiniband/ibnetdisc.h>
#include <memory>
#include "IBHost.h"
#include "IBAddress.h"
#include "IBNetfileParser.h"
#include "IBPortRegistry.h"
#include "IBHostRegistry.h"
#include "IcingaOutput.h"
#include "IBValidator.h"
#include "IBHostValidator.h"
#include "IBNetworkValidator.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

ibnd_fabric_t* scanFabric() {
    struct ibnd_config config = { 0 };
    config.max_hops = 10;
    char *ibd_ca = NULL;
    int ibd_ca_port = 0;
    ibnd_fabric_t *fabric = ibnd_discover_fabric(ibd_ca, ibd_ca_port, NULL, &config);
    if(!fabric) {
        return nullptr;
    }
    return fabric;
}

int main(int argc, const char* const* argv) {
    // Not to be confused with option_description which is something completely different
    po::options_description desc("Options");
    desc.add_options()
        ("help", "print this message")
        ("check-host", po::value<std::string>(), "check IB connectivity of a given host")
        ("netfile", po::value<std::string>()->default_value("testfile.yml"),
                   "location of the netfile (default: testfile.yml)")
        ("check-net", "check overall IB network status")
        ("clear", "clear performance counters after read (recommended)")
        ("dump", "dump all detected information about the network")
    ;
    po::variables_map options;
    po::store(po::parse_command_line(argc, argv, desc), options);
    po::notify(options);

    // Check options
    if (options.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (options.count("check-host") && options.count("check-net")) {
        std::cerr << "Only one of check-host and check-net may be specified!" << std::endl;
        return -1;
    }

    if (options.count("check-host") > 1) {
        std::cerr << "Only one host can be checked at a time!" << std::endl;
        return -1;
    }

    if ((!options.count("check-host")) && (!options.count("check-net"))) {
        std::cerr << "One of check-host and check-net must be specified!" << std::endl;
        std::cerr << desc << std::endl;
        return -1;
    }

    // Scan the IB fabric
    ibnd_fabric_t *fabric = scanFabric();
    if(!fabric) {
        std::cerr << "Couldn't scan fabric!" << std::endl;
        return -1;
    }

    // Open some port so that we can send MADs to get the statistics
    int mgmt_classes[4] = { IB_SMI_CLASS, IB_SMI_DIRECT_CLASS, IB_SA_CLASS,
                            IB_PERFORMANCE_CLASS
    };
    struct ibmad_port *ibmad_port = mad_rpc_open_port(NULL, 0, mgmt_classes, 4);

    // Parse the netfile and initialize shared objects
    std::shared_ptr<check_ib::IBNetfileParser> parser(new check_ib::IBNetfileParser(options["netfile"].as<std::string>()));
    std::shared_ptr<check_ib::IBPortRegistry> reg(new check_ib::IBPortRegistry());
    std::shared_ptr<check_ib::IBHostRegistry> hostRegistry(new check_ib::IBHostRegistry());
    std::shared_ptr<check_ib::IcingaOutput> output(new check_ib::IcingaOutput());

    // Build the object tree;
    ibnd_node_t* node = fabric->nodes;
    try {
        // Convert C magic pointer thingys into C++ classes
        std::list<std::shared_ptr<check_ib::IBHost>> hosts;
        while (node) {
            hosts.push_back(check_ib::IBHost::make_host(node, reg, ibmad_port, parser, hostRegistry, options));
            node = node->next;
        }

        // Shall we dump everything?
        if (options.count("dump")) {
            reg->isMissingSomething(true);

            for (auto iterator = hosts.begin(); iterator != hosts.end(); iterator++) {
                std::cout << "----------------------------------------------------------" << std::endl;
                std::cout << (*iterator) << std::endl;
            }
        }

        // Tell the parser that we have constructed everything we can. At this point, we should know about all hosts.
        parser->finishParsing(hostRegistry, options.count("dump") > 0);
        std::unique_ptr<check_ib::IBValidator> validator;

        if (options.count("check-host")) {
            std::unique_ptr<check_ib::IBValidator> ptr = std::unique_ptr<check_ib::IBHostValidator>(new check_ib::IBHostValidator(options["check-host"].as<std::string>()));
            validator.swap(ptr);
        } else if (options.count("check-net")) {
            std::unique_ptr<check_ib::IBValidator> ptr = std::unique_ptr<check_ib::IBNetworkValidator>(new check_ib::IBNetworkValidator(ibmad_port, options, reg));
            validator.swap(ptr);
        }

        if (validator) {
            validator->isValid(parser, output, hostRegistry);
        }
    } catch (std::exception & exception) {
        if (!output->getDidFinish()) {
            output->failUnknown() << "Caught exception in main: " << exception.what();
            output->finish();
        }
        std::cout << "Caught error in main(): " << exception.what() << std::endl;
    }

    mad_rpc_close_port(ibmad_port);
    ibnd_destroy_fabric(fabric);

    return output->getRC();
}
