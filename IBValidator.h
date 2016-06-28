//
// Created by maxf on 23.06.16.
//

#ifndef CHECK_IB_IBVALIDATOR_H
#define CHECK_IB_IBVALIDATOR_H

#include <memory>
#include "IBNetfileParser.h"

class IBHostRegistry;
class IcingaOutput;
class IBHostRegistry;

class IBValidator {
public:
    virtual bool isValid(std::shared_ptr<IBNetfileParser>, std::shared_ptr<IcingaOutput>, std::shared_ptr<IBHostRegistry>) throw(IBException) = 0;
    const bool hasLinkTo(const std::shared_ptr<IBHost>, const std::shared_ptr<IBHost>) const;
};


#endif //CHECK_IB_IBVALIDATOR_H
