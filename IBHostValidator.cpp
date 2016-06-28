//
// Created by maxf on 23.06.16.
//

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
