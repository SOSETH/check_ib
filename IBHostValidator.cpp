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

#include "IBHostValidator.h"
#include "IBHostRegistry.h"
#include "IBNetfileParser.h"
#include "IBHost.h"
#include "IBPort.h"
#include "IcingaOutput.h"

namespace check_ib {

    IBHostValidator::IBHostValidator(const std::string &hostname) noexcept : IBValidator(), myHostName(hostname) { }

    IBHostValidator::IBHostValidator(const std::string &&hostname) noexcept : IBValidator(), myHostName(hostname) { }

    bool IBHostValidator::isValid(std::shared_ptr<IBNetfileParser> parser, std::shared_ptr<IcingaOutput> output,
                                  std::shared_ptr<IBHostRegistry> registry) throw(IBException) {
        bool isOK = true;
        if (!registry->has(myHostName)) {
            output->failCritical() << "Coudln't find host to check!";
            return false;
        }
        auto me = (*registry)[myHostName];
        output->setIBHostDetail(me);

        // Check that all links are up
        auto expectedLinks = parser->getExpectedLinks();
        for (auto link = expectedLinks.begin(); link != expectedLinks.end(); link++) {
            if (link->first == me) {
                if (!hasLinkTo(link->first, link->second)) {
                    isOK = false;
                    output->failWarning() << "Missing expected link from Host " << link->first->getName()
                                            << " to " << link->second->getName() << "!";
                }
            } else if (link->second == me) {
                if (!hasLinkTo(link->second, link->first)) {
                    isOK = false;
                    output->failWarning() << "Missing expected link from Host " << link->second->getName()
                                            << " to " << link->first->getName() << "!";
                }
            }
        }

        if (isOK) {
            std::ostringstream str;
            bool hasErrors = false;
            // Check that all links are at their maximum reported speed
            auto ports = me->getPorts();
            for (auto port = ports.begin(); port != ports.end(); port++) {
                // We only check active ports
                if ((*port)->getStateLogical() != IBPort::LogPortState::ACTIVE)
                    continue;

                IBPort::LinkSpeed maxSpeed = (*port)->getLinkSpeedEnabled();
                if (auto peer = (*port)->getPeer().lock()) {
                    if (peer->getLinkSpeedEnabled() < maxSpeed)
                        maxSpeed = peer->getLinkSpeedEnabled();
                }
                if ((*port)->getLinkSpeedActive() < maxSpeed) {
                    hasErrors = true;
                    str << "Link speed too low (" << (*port)->getLinkSpeedActive() << " out of " << maxSpeed
                        << "on link " << (*port)->getPortNum() << " of host " << me->getName() << std::endl;
                }
            }
            if (hasErrors) {
                output->failWarning() << str.str();
                isOK = false;
            }
        }

        output->printPerformanceData(me);
        output->finish();

        return isOK;
    }
}
