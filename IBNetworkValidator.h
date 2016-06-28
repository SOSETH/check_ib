//
// Created by maxf on 26.06.16.
//

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
