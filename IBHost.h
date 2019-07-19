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
                                                 boost::program_options::variables_map &);

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
