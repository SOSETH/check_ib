//
// Created by maxf on 23.06.16.
//

#include <iostream>
#include "IcingaOutput.h"

IcingaOutput::IcingaOutput() {
    rc = -1;
}

void IcingaOutput::setIBHostDetail(std::shared_ptr<IBHost> host) {
    dumpFun = [host] () {
        std::cout << host << std::endl;
    };
}

void IcingaOutput::failCritical(std::string &&reason) {
    std::cout << "CRITICAL: " << reason << std::endl;
    if (dumpFun)
        dumpFun();
    if (rc != -1)
        throw IcingaOutputException("Return code has already been set - check your code!");
    rc = 2;
}

