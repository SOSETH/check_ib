//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBPORTREGISTRY_H
#define CHECK_IB_IBPORTREGISTRY_H

#include <map>
#include <list>

class IBPort;

class IBPortRegistry {
public:
    void registerPort(IBPort &);
    IBPort &getPortWithGUID(const uint64_t);
    bool hasPortWithGUID(const uint64_t) const;
    void registerInterestFor(uint64_t guid, IBPort&);

protected:
    std::map<uint64_t, IBPort*> portRegistry;
    std::map<uint64_t, std::list<IBPort*>> interestRegistry;
};


#endif //CHECK_IB_IBPORTREGISTRY_H
