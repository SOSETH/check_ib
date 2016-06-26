//
// Created by maxf on 23.06.16.
//

#include <iostream>
#include "IcingaOutput.h"
#include "IBHost.h"
#include "IBPort.h"

IcingaOutput::IcingaOutput() {
    rc = -1;
}

void IcingaOutput::setIBHostDetail(std::shared_ptr<IBHost> host) {
    dumpFun = [host] () {
        std::cout << host << std::endl;
    };
}

void IcingaOutput::failCritical(std::string &&reason) {
    std::cout << "CRITICAL: " << reason;
    rc = 2;
}

void IcingaOutput::failWarning(std::string &&reason) {
    if (rc != -1)
        return;
    std::cout << "WARNING: " << reason;
    rc = 1;
}

void IcingaOutput::failUnknown(std::string &&reason) {
    if (rc != -1)
        return;
    std::cout << "UNKNOWN: " << reason;
    rc = 3;
}

void IcingaOutput::finish() {
    std::cout << std::endl;
    if (didFinish)
        return;
    if (dumpFun)
        dumpFun();
    didFinish = true;
}

void IcingaOutput::printPerformanceData(std::shared_ptr<IBHost> host) {
    auto ports = host->getPorts();
    auto description = IBPort::getAttributeDescriptions();
    for (auto port = ports.begin(); port != ports.end(); port++) {
        for (auto attribute = description.begin(); attribute != description.end(); attribute++) {
            if (isFirstPerformanceMetric) {
                isFirstPerformanceMetric = false;
                std::cout << " | ";
            } else
                std::cout << " ";
            std::cout << "'port " << (*port)->getPortNum() << " " << attribute->second << "'=" << (**port)[attribute->first];
            // Print "UOM", see https://nagios-plugins.org/doc/guidelines.html#PLUGOUTPUT
            switch (attribute->first) {
                case IBPort::PortAttribute::RX_BYTES:
                case IBPort::PortAttribute::TX_BYTES:
                    std::cout << "B";
                    break;
                default:
                    break;
            }
        }
    }
}