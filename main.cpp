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
        ("clear", "clear performance counters after read (recommended)")
        ("dump", "dump all detected information about the network")
    ;
    std::shared_ptr<po::variables_map> options(new po::variables_map());
    po::store(po::parse_command_line(argc, argv, desc), *options);
    po::notify(*options);

    if (options->count("help")) {
        std::cout << desc << std::endl;
        return 0;
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

    // Parse the netfile
    std::shared_ptr<IBNetfileParser> parser(new IBNetfileParser("testfile.yml"));
    std::shared_ptr<IBPortRegistry> reg(new IBPortRegistry());
    std::shared_ptr<IBHostRegistry> hostRegistry(new IBHostRegistry());
    std::shared_ptr<IcingaOutput> output(new IcingaOutput());

    // Build the object tree;
    ibnd_node_t* node = fabric->nodes;
    try {
        std::list<std::shared_ptr<IBHost>> hosts;
        while (node) {
            hosts.push_back(IBHost::make_host(node, reg, ibmad_port, parser, hostRegistry, options));
            node = node->next;
        }

        if (options->count("dump")) {
            reg->isMissingSomething(true);

            for (auto iterator = hosts.begin(); iterator != hosts.end(); iterator++) {
                std::cout << (*iterator) << std::endl;
            }
        }

        parser->finishParsing(hostRegistry, true);
        IBValidator* validator = nullptr;

        if (options->count("check-host")) {
            validator = new IBHostValidator((*options)["check-host"].as<std::string>(), parser, output);
        }

        if (validator) {
            validator->isValid(hostRegistry);
            delete validator;
        }
    } catch (const char *err) {
        std::cerr << "Caught error in main(): " << err << std::endl;
    } catch (std::exception & exception) {
        std::cerr << "Caught error in main(): " << exception.what() << std::endl;
    }

    mad_rpc_close_port(ibmad_port);
    ibnd_destroy_fabric(fabric);

    return output->getRC();
}