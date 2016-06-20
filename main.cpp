#include <iostream>

#include <list>
#include <infiniband/ibnetdisc.h>
#include <memory>
#include "IBHost.h"

int main() {
    struct ibnd_config config = { 0 };
    config.max_hops = 10;
    char *ibd_ca = NULL;
    int ibd_ca_port = 0;
    ibnd_fabric_t *fabric = ibnd_discover_fabric(ibd_ca, ibd_ca_port, NULL, &config);
    if(!fabric) {
        std::cerr << "Couldn't scan fabric!" << std::endl;
        return -1;
    }

    ibnd_node_t* node = fabric->nodes;
    try {
        std::list<std::shared_ptr<IBHost>> hosts;
        auto reg = std::make_shared<IBPortRegistry>();
        while (node) {
            hosts.push_back(IBHost::make_host(node, reg));
            node = node->next;
        }

        for (auto iterator = hosts.begin(); iterator != hosts.end(); iterator++) {
            std::cout << (*iterator) << std::endl;
        }
    } catch (const char *err) {
        std::cerr << "Caught error in main(): " << err << std::endl;
    }

    ibnd_destroy_fabric(fabric);

    return 0;
}