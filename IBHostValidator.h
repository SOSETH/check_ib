//
// Created by maxf on 23.06.16.
//

#ifndef CHECK_IB_IBHOSTVALIDATOR_H
#define CHECK_IB_IBHOSTVALIDATOR_H

#include "IBValidator.h"

class IBHostValidator : public IBValidator {
private:
    std::string myHostName;
    std::shared_ptr<IBHostRegistry> registry;

public:
    IBHostValidator(const std::string&, std::shared_ptr<IBNetfileParser>, std::shared_ptr<IcingaOutput>, std::shared_ptr<IBHostRegistry>);
    IBHostValidator(const std::string&&, std::shared_ptr<IBNetfileParser>, std::shared_ptr<IcingaOutput>, std::shared_ptr<IBHostRegistry>);

    virtual bool isValid() throw(IBException);
};


#endif //CHECK_IB_IBHOSTVALIDATOR_H
