//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBPORTREGISTRY_H
#define CHECK_IB_IBPORTREGISTRY_H

#include <map>
#include <list>
#include <memory>

class IBPort;

class IBPortRegistry {
public:
    void registerPort(std::shared_ptr<IBPort>);
    std::shared_ptr<IBPort> getPortWithGUID(const uint64_t);
    bool hasPortWithGUID(const uint64_t) const;
    void registerInterestFor(uint64_t guid, std::shared_ptr<IBPort>);

protected:
    std::map<uint64_t, std::shared_ptr<IBPort>> portRegistry;
    std::map<uint64_t, std::list<std::shared_ptr<IBPort>>> interestRegistry;
};


#endif //CHECK_IB_IBPORTREGISTRY_H
