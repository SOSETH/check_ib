//
// Created by maxf on 23.06.16.
//

#ifndef CHECK_IB_IBVALIDATOR_H
#define CHECK_IB_IBVALIDATOR_H

#include <memory>
#include "IBNetfileParser.h"

class IBHostRegistry;
class IcingaOutput;

class IBValidator {
public:
    IBValidator(std::shared_ptr<IBNetfileParser> parser, std::shared_ptr<IcingaOutput> output);
    virtual bool isValid() throw(IBException) = 0;
    const bool hasLinkTo(const std::shared_ptr<IBHost>, const std::shared_ptr<IBHost>) const;

protected:
    std::shared_ptr<IBNetfileParser> parser;
    std::shared_ptr<IcingaOutput> output;
};


#endif //CHECK_IB_IBVALIDATOR_H
