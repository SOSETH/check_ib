//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBHOST_H
#define CHECK_IB_IBHOST_H

#include <list>
#include <ibnetdisc.h>
#include "IBPortRegistry.h"

class IBPort;

class IBHost {
public:
    IBHost(ibnd_node_t* host, IBPortRegistry&);
    IBHost();

    unsigned int getNumPorts() const {
        return numPorts;
    }

    uint64_t getGUID() const {
        return guid;
    }

    const std::list<IBPort*> &getPorts() const {
        return ports;
    }

protected:
    uint64_t guid;
    unsigned int numPorts;
    std::list<IBPort*> ports;
    IBPortRegistry registry;
};

std::ostream& operator<<(std::ostream&, const IBHost&);
#endif //CHECK_IB_IBHOST_H
