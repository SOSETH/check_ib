//
// Created by maxf on 26.06.16.
//

#ifndef CHECK_IB_IBNETWORKVALIDATOR_H
#define CHECK_IB_IBNETWORKVALIDATOR_H

#include <infiniband/ibnetdisc.h>
#include <boost/program_options.hpp>
#include "IBValidator.h"
#include "IBPortRegistry.h"

class IBNetworkValidator : public IBValidator {
public:
    class IBNetworkValidatorException : public virtual IBException {
    public:
        IBNetworkValidatorException(const std::string &arg) : IBException(arg), domain_error(arg) { }
        IBNetworkValidatorException(const char *arg) : IBException(arg), domain_error(arg)  { }
    };
    IBNetworkValidator(struct ibmad_port *ibmad_port, boost::program_options::variables_map& options,
                       std::shared_ptr<IBPortRegistry> portRegistry);
    virtual bool isValid(std::shared_ptr<IBNetfileParser>, std::shared_ptr<IcingaOutput>,
                         std::shared_ptr<IBHostRegistry>) throw(IBException);

private:
    struct ibmad_port *ibmad_port;
    void dumpBuffer(uint8_t*, int);
    boost::program_options::variables_map options;
    std::shared_ptr<IBPortRegistry> portRegistry;
    std::shared_ptr<IBHostRegistry> hostRegistry;
};


#endif //CHECK_IB_IBNETWORKVALIDATOR_H
