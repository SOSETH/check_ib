//
// Created by maxf on 23.06.16.
//

#ifndef CHECK_IB_IBHOSTREGISTRY_H
#define CHECK_IB_IBHOSTREGISTRY_H

#include <map>
#include <memory>

class IBHost;

class IBHostRegistry {
private:
    std::map<uint64_t, std::shared_ptr<IBHost>> hostByGUID;
    std::map<std::string, std::shared_ptr<IBHost>> hostByName;

public:
    void addIBHost(std::shared_ptr<IBHost>);
    std::shared_ptr<IBHost> operator[](const uint64_t);
    std::shared_ptr<IBHost> operator[](const std::string&);
    std::shared_ptr<IBHost> operator[](const std::string&&);
    bool has(const std::string&) const;
    const std::map<std::string, std::shared_ptr<IBHost>> & getAllByName() const {
        return hostByName;
    };
};


#endif //CHECK_IB_IBHOSTREGISTRY_H
