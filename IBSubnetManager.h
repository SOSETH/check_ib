//
// Created by maxf on 27.06.16.
//

#ifndef CHECK_IB_IBSUBNETMANAGER_H
#define CHECK_IB_IBSUBNETMANAGER_H

#include <memory>
#include "IBException.h"

class IBPort;
class IBPortRegistry;

class IBSubnetManager {
public:
    enum SMState {
        NOT_ACTIVE=0,
        DISCOVERING=1,
        STANDBY=2,
        MASTER=3
    };

private:
    std::shared_ptr<IBPort> port;
    uint32_t actCounter;
    unsigned int priority;
    SMState state;

public:
    const std::shared_ptr<IBPort> &getPort() const {
        return port;
    }

    uint32_t getActCounter() const {
        return actCounter;
    }

    unsigned int getPriority() const {
        return priority;
    }

    SMState getState() const {
        return state;
    }

    class IBSubnetManagerException : public virtual IBException {
    public:
        IBSubnetManagerException(const std::string &arg) : IBException(arg), domain_error(arg) { }
        IBSubnetManagerException(const char *arg) : IBException(arg), domain_error(arg)  { }
    };

    IBSubnetManager(uint8_t*, std::shared_ptr<IBPortRegistry>);

};

std::ostream& operator<<(std::ostream&, const IBSubnetManager::SMState);
std::ostream& operator<<(std::ostream&, const IBSubnetManager&);

#endif //CHECK_IB_IBSUBNETMANAGER_H
