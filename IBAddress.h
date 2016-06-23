//
// Created by maxf on 22.06.16.
//

#ifndef CHECK_IB_IBADDRESS_H
#define CHECK_IB_IBADDRESS_H

#include <ibnetdisc.h>
#include <ostream>

class IBPort;

class IBAddress {
private:
    uint64_t guid;
    uint16_t lid;
    int num;

public:
    uint64_t getGuid() const {
        return guid;
    }

    uint16_t getLid() const {
        return lid;
    }

    int getNum() const {
        return num;
    }

    IBAddress(ibnd_port_t* port);
    IBAddress(IBPort & port);
    bool operator<(const IBAddress& other) const;
};

std::ostream& operator<<(std::ostream&, const IBAddress &);

#endif //CHECK_IB_IBADDRESS_H
