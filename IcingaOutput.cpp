/*
 * Copyright (C) 2016  Maximilian Falkenstein <mfalkenstein@sos.ethz.ch>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include "IcingaOutput.h"
#include "IBHost.h"
#include "IBPort.h"
#include "IBSubnetManager.h"

namespace check_ib {
    IcingaOutput::IcingaOutput() noexcept {
        rc = -1;
    }

    void IcingaOutput::setIBHostDetail(std::shared_ptr<IBHost> host) noexcept {
        dumpFun = [host]() {
            std::cout << host << std::endl;
        };
    }


    void IcingaOutput::setIBSubnetManagersDetail(
            std::shared_ptr<std::map<uint64_t, std::shared_ptr<IBSubnetManager>>> smMap) noexcept {
        dumpFun = [smMap]() {
            for (auto sm = smMap->begin(); sm != smMap->end(); sm++) {
                std::cout << *(sm->second);
            }
        };
    }

    std::ostream &IcingaOutput::failCritical() noexcept {
        if (rc < 2)
            rc = 2;
        return outputMessage << std::endl << " Crit: ";
    }

    std::ostream &IcingaOutput::failWarning() noexcept {
        if (rc < 1)
            rc = 1;
        return outputMessage << std::endl << " Warn: ";
    }

    std::ostream &IcingaOutput::failUnknown() noexcept {
        if (rc < 3)
            rc = 3;
        return outputMessage << std::endl << " Unknown: ";
    }

    void IcingaOutput::finish() noexcept {
        if (didFinish)
            return;
        if (rc == -1)
            rc = 0;
        switch (rc) {
            case 0:
                std::cout << "OK";
                break;
            case 1:
                std::cout << "WARNING";
                break;
            case 2:
                std::cout << "CRITICAL";
                break;
            default:
                std::cout << "UNKNOWN";
                break;
        }
        std::cout << performanceData.str();
        std::cout << outputMessage.str() << std::endl;

        if (rc != 0 && dumpFun)
            dumpFun();
        didFinish = true;
    }

    void IcingaOutput::printPerformanceData(std::shared_ptr<IBHost> host) noexcept {
        auto ports = host->getPorts();
        auto description = IBPort::getAttributeDescriptions();
        for (auto port = ports.begin(); port != ports.end(); port++) {
            for (auto attribute = description.begin(); attribute != description.end(); attribute++) {
                if (isFirstPerformanceMetric) {
                    isFirstPerformanceMetric = false;
                    performanceData << " | ";
                } else
                    performanceData << " ";
                performanceData << "'port " << (*port)->getPortNum() << " " << attribute->second << "'=" <<
                (**port)[attribute->first];
                // Print "UOM", see https://nagios-plugins.org/doc/guidelines.html#PLUGOUTPUT
                switch (attribute->first) {
                    case IBPort::PortAttribute::RX_BYTES:
                    case IBPort::PortAttribute::TX_BYTES:
                        performanceData << "B";
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void IcingaOutput::printPerformanceData(
            std::shared_ptr<std::map<uint64_t, std::shared_ptr<IBSubnetManager>>> smMap) noexcept {
        for (auto smEntry = smMap->begin(); smEntry != smMap->end(); smEntry++) {
            auto sm = smEntry->second;
            if (isFirstPerformanceMetric) {
                isFirstPerformanceMetric = false;
                performanceData << " | ";
            } else
                performanceData << " ";
            std::ostringstream prefix;
            if (auto host = sm->getPort()->getHost().lock()) {
                prefix << "'SM " << host->getName() << "-" << sm->getPort()->getPortNum() << " ";
            } else {
                prefix << "'SM " << std::hex << sm->getPort()->getGuid() << " LID " << sm->getPort()->getLid() << " ";
            }
            std::string pstr = prefix.str();
            performanceData << pstr << "ACT'=" << sm->getActCounter() << "c "
            << pstr << "priority'=" << sm->getPriority() << " "
            << pstr << "state'=" << static_cast<int>(sm->getState());
        }
    }
}
