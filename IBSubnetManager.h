//
// Created by maxf on 27.06.16.
//

#ifndef CHECK_IB_IBSUBNETMANAGER_H
#define CHECK_IB_IBSUBNETMANAGER_H

#include <memory>
#include "IBException.h"

namespace check_ib {
    class IBPort;
    class IBPortRegistry;

    class IBSubnetManager {
    public:
        enum SMState {
            NOT_ACTIVE = 0,
            DISCOVERING = 1,
            STANDBY = 2,
            MASTER = 3
        };

    private:
        std::shared_ptr<IBPort> port;
        uint32_t actCounter;
        unsigned int priority;
        SMState state;

    public:
        const std::shared_ptr<IBPort> &getPort() const noexcept {
            return port;
        }

        uint32_t getActCounter() const noexcept {
            return actCounter;
        }

        unsigned int getPriority() const noexcept {
            return priority;
        }

        SMState getState() const noexcept {
            return state;
        }

        class IBSubnetManagerException : public virtual IBException {
        public:
            IBSubnetManagerException(const std::string &arg) noexcept : IBException(arg), domain_error(arg) { }
            IBSubnetManagerException(const char *arg) noexcept : IBException(arg), domain_error(arg) { }
        };

        IBSubnetManager(uint8_t *, std::shared_ptr<IBPortRegistry>) throw(IBSubnetManagerException);
    };

    std::ostream &operator<<(std::ostream &, const IBSubnetManager::SMState) noexcept;
    std::ostream &operator<<(std::ostream &, const IBSubnetManager &) noexcept;
}

#endif //CHECK_IB_IBSUBNETMANAGER_H
