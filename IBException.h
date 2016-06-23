//
// Created by maxf on 22.06.16.
//

#ifndef CHECK_IB_IBEXCEPTION_H
#define CHECK_IB_IBEXCEPTION_H


#include <stdexcept>

class IBException : public virtual std::domain_error{

public:
    IBException(const char *arg) : domain_error(arg) {}
    IBException(const std::string &arg) : domain_error(arg) { }
};


#endif //CHECK_IB_IBEXCEPTION_H
