#include <iostream>

#include <list>
#include <infiniband/ibnetdisc.h>
#include <memory>
#include "IBHost.h"
#include "IBNetfileParser.h"
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
    std::shared_ptr<IBNetfileParser> parser(new IBNetfileParser(options["netfile"].as<std::string>()));
    std::shared_ptr<IBPortRegistry> reg(new IBPortRegistry());
    std::shared_ptr<IBHostRegistry> hostRegistry(new IBHostRegistry());
    std::shared_ptr<IcingaOutput> output(new IcingaOutput());

    // Build the object tree;
    ibnd_node_t* node = fabric->nodes;
    try {
        // Convert C magic pointer thingys into C++ classes
        std::list<std::shared_ptr<IBHost>> hosts;
        while (node) {
            hosts.push_back(IBHost::make_host(node, reg, ibmad_port, parser, hostRegistry, options));
            node = node->next;
        }

        // Shall we dump everything?
        if (options.count("dump")) {
            reg->isMissingSomething(true);

            for (auto iterator = hosts.begin(); iterator != hosts.end(); iterator++) {
                std::cout << (*iterator) << std::endl;
            }
        }

        // Tell the parser that we have constructed everything we can. At this point, we should know about all hosts.
        parser->finishParsing(hostRegistry, options.count("dump") > 0);
        std::unique_ptr<IBValidator> validator;

        if (options.count("check-host")) {
            std::unique_ptr<IBValidator> ptr = std::unique_ptr<IBHostValidator>(new IBHostValidator(options["check-host"].as<std::string>()));
            validator.swap(ptr);
        } else if (options.count("check-net")) {
            std::unique_ptr<IBValidator> ptr = std::unique_ptr<IBNetworkValidator>(new IBNetworkValidator(ibmad_port, options, reg));
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