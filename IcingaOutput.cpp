//
// Created by maxf on 23.06.16.
//

#include <iostream>
#include "IcingaOutput.h"
#include "IBHost.h"
#include "IBPort.h"
#include "IBSubnetManager.h"

IcingaOutput::IcingaOutput() {
    rc = -1;
}

void IcingaOutput::setIBHostDetail(std::shared_ptr<IBHost> host) {
    dumpFun = [host] () {
        std::cout << host << std::endl;
    };
}


void IcingaOutput::setIBSubnetManagersDetail(std::shared_ptr<std::map<uint64_t, std::shared_ptr<IBSubnetManager>>> smMap) {
    dumpFun = [smMap] () {
        for (auto sm = smMap->begin(); sm != smMap->end(); sm++) {
            std::cout << *(sm->second);
        }
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
    if (rc != 0 && dumpFun)
        dumpFun();
    didFinish = true;
}

void IcingaOutput::printPerformanceData(std::shared_ptr<IBHost> host) {
    if (rc == -1) {
        rc = 0;
        std::cout << "OK";
    }

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

void IcingaOutput::printPerformanceData(std::shared_ptr<std::map<uint64_t, std::shared_ptr<IBSubnetManager>>> smMap) {
    if (rc == -1) {
        rc = 0;
        std::cout << "OK";
    }

    for (auto smEntry = smMap->begin(); smEntry != smMap->end(); smEntry++) {
        auto sm = smEntry->second;
        if (isFirstPerformanceMetric) {
            isFirstPerformanceMetric = false;
            std::cout << " | ";
        } else
            std::cout << " ";
        std::ostringstream prefix;
        if (auto host = sm->getPort()->getHost().lock()) {
            prefix << "'SM " << host->getName() << "-" << sm->getPort()->getPortNum() << " ";
        } else {
            prefix << "'SM " << std::hex << sm->getPort()->getGuid() << " LID " << sm->getPort()->getLid() << " ";
        }
        std::string pstr = prefix.str();
        std::cout << pstr << "ACT'=" << sm->getActCounter() << "c "
                << pstr << "priority'=" << sm->getPriority() << " "
                << pstr << "state'=" << static_cast<int>(sm->getState());
    }
}