//
// Created by maxf on 22.06.16.
//

#ifndef CHECK_IB_IBADDRESS_H
#define CHECK_IB_IBADDRESS_H

#include <ibnetdisc.h>
#include <ostream>

namespace check_ib {
    class IBPort;

    class IBAddress {
    private:
        uint64_t guid;
        uint16_t lid;
        int num;

    public:
        uint64_t getGuid() const noexcept {
            return guid;
        }

        uint16_t getLid() const noexcept {
            return lid;
        }

        int getNum() const noexcept {
            return num;
        }

        IBAddress(ibnd_port_t *port) noexcept;

        IBAddress(IBPort &port) noexcept;

        bool operator<(const IBAddress &other) const noexcept;
    };

    std::ostream &operator<<(std::ostream &, const IBAddress &) noexcept;
}

#endif //CHECK_IB_IBADDRESS_H
