//
// Created by maxf on 26.06.16.
//

#ifndef CHECK_IB_IBNETWORKVALIDATOR_H
#define CHECK_IB_IBNETWORKVALIDATOR_H

#include <infiniband/ibnetdisc.h>
#include "IBValidator.h"

class IBNetworkValidator : public IBValidator {
public:
    class IBNetworkValidatorException : public virtual IBException {
    public:
        IBNetworkValidatorException(const std::string &arg) : IBException(arg), domain_error(arg) { }
        IBNetworkValidatorException(const char *arg) : IBException(arg), domain_error(arg)  { }
    };
    IBNetworkValidator(std::shared_ptr<IBNetfileParser>, std::shared_ptr<IcingaOutput>, struct ibmad_port *ibmad_port);
    virtual bool isValid() throw(IBException);

private:
    struct ibmad_port *ibmad_port;
};


#endif //CHECK_IB_IBNETWORKVALIDATOR_H
