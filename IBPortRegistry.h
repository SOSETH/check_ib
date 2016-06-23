//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBPORTREGISTRY_H
#define CHECK_IB_IBPORTREGISTRY_H

#include <map>
#include <list>
#include <memory>
#include "IBAddress.h"

class IBPort;

class IBPortRegistry {
public:
    void registerPort(std::shared_ptr<IBPort>);
    std::shared_ptr<IBPort> getPortWithAddress(const IBAddress);
    bool hasPortWithAddress(const IBAddress) const;
    void registerInterestFor(IBAddress guid, std::shared_ptr<IBPort>);
    bool isMissingSomething(const bool printOnErr) const;

protected:
    std::map<IBAddress, std::shared_ptr<IBPort>> portRegistry;
    std::map<IBAddress, std::shared_ptr<IBPort>> interestRegistry;
};


#endif //CHECK_IB_IBPORTREGISTRY_H
