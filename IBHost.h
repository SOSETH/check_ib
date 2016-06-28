//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBHOST_H
#define CHECK_IB_IBHOST_H

#include <list>
#include <ibnetdisc.h>
#include <memory>
#include <string>
#include <boost/program_options.hpp>
#include "IBException.h"

namespace check_ib {
    class IBPort;
    class IBHostRegistry;
    class IBPortRegistry;
    class IBNetfileParser;

    class IBHost {
    public:
        static std::shared_ptr<IBHost> make_host(ibnd_node_t *, std::shared_ptr<IBPortRegistry>,
                                                 struct ibmad_port *, std::shared_ptr<IBNetfileParser>,
                                                 std::shared_ptr<IBHostRegistry>,
                                                 boost::program_options::variables_map &) throw(IBException);

        unsigned int getNumPorts() const noexcept {
            return numPorts;
        }

        uint64_t getGUID() const noexcept {
            return guid;
        }

        const std::list<std::shared_ptr<IBPort>> getPorts() const noexcept {
            return ports;
        }

        const std::string &getName() const noexcept {
            return name;
        }

    protected:
        IBHost(std::shared_ptr<IBPortRegistry>) noexcept;

        uint64_t guid;
        unsigned int numPorts;
        std::list<std::shared_ptr<IBPort>> ports;
        std::shared_ptr<IBPortRegistry> registry;
        std::string name;
    };

    std::ostream &operator<<(std::ostream &, const IBHost *) noexcept;
}
#endif //CHECK_IB_IBHOST_H
