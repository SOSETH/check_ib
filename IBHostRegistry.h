//
// Created by maxf on 23.06.16.
//

#ifndef CHECK_IB_IBHOSTREGISTRY_H
#define CHECK_IB_IBHOSTREGISTRY_H

#include <map>
#include <memory>
#include "IBException.h"

namespace check_ib {
    class IBHost;

    class IBHostRegistry {
    private:
        std::map<uint64_t, std::shared_ptr<IBHost>> hostByGUID;
        std::map<std::string, std::shared_ptr<IBHost>> hostByName;

    public:
        void addIBHost(std::shared_ptr<IBHost>) throw(IBException);

        std::shared_ptr<IBHost> operator[](const uint64_t) noexcept;

        std::shared_ptr<IBHost> operator[](const std::string &) noexcept;

        std::shared_ptr<IBHost> operator[](const std::string &&) noexcept;

        bool has(const std::string &) const noexcept;

        const std::map<std::string, std::shared_ptr<IBHost>> &getAllByName() const noexcept {
            return hostByName;
        };
    };
}

#endif //CHECK_IB_IBHOSTREGISTRY_H
