//
// Created by maxf on 07.06.16.
//

#ifndef CHECK_IB_IBPORTREGISTRY_H
#define CHECK_IB_IBPORTREGISTRY_H

#include <map>
#include <list>
#include <memory>
#include "IBException.h"

namespace check_ib {
    class IBPort;
    class IBAddress;

    class IBPortRegistry {
    public:
        void registerPort(std::shared_ptr<IBPort>) noexcept;

        std::shared_ptr<IBPort> getPortWithAddress(const IBAddress) noexcept;

        bool hasPortWithAddress(const IBAddress) const noexcept;

        void registerInterestFor(IBAddress& guid, std::shared_ptr<IBPort>) throw(IBException);

        bool isMissingSomething(const bool printOnErr) const noexcept;

        std::shared_ptr<IBPort> operator[](const uint64_t) noexcept;

    protected:
        std::map<IBAddress, std::shared_ptr<IBPort>> portRegistry;
        std::map<IBAddress, std::shared_ptr<IBPort>> interestRegistry;
        std::map<uint64_t, std::shared_ptr<IBPort>> portByGUIDRegistry;
    };
}

#endif //CHECK_IB_IBPORTREGISTRY_H
