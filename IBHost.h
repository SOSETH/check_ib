//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBHOST_H
#define CHECK_IB_IBHOST_H

#include <list>
#include <ibnetdisc.h>
#include <memory>
#include <string>
#include "IBPortRegistry.h"
#include "IBNetfileParser.h"

class IBPort;

class IBHost : public std::enable_shared_from_this<IBHost>{
public:
    static std::shared_ptr<IBHost> make_host(ibnd_node_t*, std::shared_ptr<IBPortRegistry>,
                                             struct ibmad_port *ibmad_port, std::shared_ptr<IBNetfileParser> nf);

    unsigned int getNumPorts() const {
        return numPorts;
    }

    uint64_t getGUID() const {
        return guid;
    }

    const std::list<std::shared_ptr<IBPort>> getPorts() const {
        return ports;
    }

    const std::string & getName() const {
        return name;
    }

protected:
    IBHost(std::shared_ptr<IBPortRegistry>);
    uint64_t guid;
    unsigned int numPorts;
    std::list<std::shared_ptr<IBPort>> ports;
    std::shared_ptr<IBPortRegistry> registry;
    std::string name;
};

std::ostream& operator<<(std::ostream&, const IBHost*);
#endif //CHECK_IB_IBHOST_H
