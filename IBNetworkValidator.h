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

#ifndef CHECK_IB_IBNETWORKVALIDATOR_H
#define CHECK_IB_IBNETWORKVALIDATOR_H

#include <infiniband/ibnetdisc.h>
#include <boost/program_options.hpp>
#include "IBException.h"
#include "IBValidator.h"

namespace check_ib {
    class IBPortRegistry;

    class IBNetworkValidator : public IBValidator {
    public:
        class IBNetworkValidatorException : public virtual IBException {
        public:
            IBNetworkValidatorException(const std::string &arg) noexcept : IBException(arg), domain_error(arg) { }

            IBNetworkValidatorException(const char *arg) noexcept : IBException(arg), domain_error(arg) { }
        };

        IBNetworkValidator(struct ibmad_port *ibmad_port, boost::program_options::variables_map &options,
                           std::shared_ptr<IBPortRegistry> portRegistry) noexcept;

        virtual bool isValid(std::shared_ptr<IBNetfileParser>, std::shared_ptr<IcingaOutput>,
                             std::shared_ptr<IBHostRegistry>) throw(IBException);

    private:
        struct ibmad_port *ibmad_port;

        void dumpBuffer(uint8_t *, int);

        boost::program_options::variables_map options;
        std::shared_ptr<IBPortRegistry> portRegistry;
    };
}

#endif //CHECK_IB_IBNETWORKVALIDATOR_H
