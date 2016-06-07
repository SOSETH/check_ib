#include <iostream>

#include <list>
#include <infiniband/ibnetdisc.h>
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
    std::list<IBHost> hosts;
    IBPortRegistry reg;
    while (node) {
        hosts.push_back(IBHost(node, reg));
        node = node->next;
    }

    for (auto iterator = hosts.begin(); iterator != hosts.end(); ++iterator) {
        std::cout << (*iterator) << std::endl;
    }

    ibnd_destroy_fabric(fabric);

    return 0;
}