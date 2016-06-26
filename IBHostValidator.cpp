//
// Created by maxf on 23.06.16.
//

#include "IBHostValidator.h"
#include "IBHostRegistry.h"
#include "IBNetfileParser.h"
#include "IBHost.h"
#include "IBPort.h"
#include "IcingaOutput.h"

IBHostValidator::IBHostValidator(const std::string& hostname, std::shared_ptr<IBNetfileParser> parser,
                                 std::shared_ptr<IcingaOutput> output, std::shared_ptr<IBHostRegistry> registry) :
        IBValidator(parser, output), myHostName(hostname), registry(registry) {}
IBHostValidator::IBHostValidator(const std::string&& hostname, std::shared_ptr<IBNetfileParser> parser,
                                 std::shared_ptr<IcingaOutput> output, std::shared_ptr<IBHostRegistry> registry) :
        IBValidator(parser, output), myHostName(hostname), registry(registry) {}

bool IBHostValidator::isValid() throw(IBException) {
    bool isOK=true;
    if (!registry->has(myHostName)) {
        output->failCritical("Coudln't find host to check!");
        return false;
    }
    auto me = (*registry)[myHostName];
    output->setIBHostDetail(me);

    // Check that all links are up
    auto expectedLinks = parser->getExpectedLinks();
    for (auto link = expectedLinks.begin(); link != expectedLinks.end(); link++) {
        if (link->first == me) {
            if (!hasLinkTo(me, link->second)) {
                isOK = false;
                output->failWarning("Missing expected link!");
            }
        } else if (link->second == me) {
            if (!hasLinkTo(link->second, me)) {
                isOK = false;
                output->failWarning("Missing expected link!");
            }
        }
    }

    if (isOK) {
        std::ostringstream str;
        bool hasErrors = false;
        // Check that all links are at their maximum reported speed
        const auto hosts = registry->getAllByName();
        for (auto hostptr = hosts.begin(); hostptr != hosts.end(); hostptr++) {
            auto host = hostptr->second;
            auto ports = host->getPorts();
            for (auto port = ports.begin(); port != ports.end(); port++) {
                IBPort::LinkSpeed maxSpeed = (*port)->getLinkSpeedEnabled();
                if (auto peer = (*port)->getPeer().lock()) {
                    if (peer->getLinkSpeedEnabled() < maxSpeed)
                        maxSpeed = peer->getLinkSpeedEnabled();
                }
                if ((*port)->getLinkSpeedActive() < maxSpeed) {
                    hasErrors = true;
                    str << "Link speed too low (" << (*port)->getLinkSpeedActive() << " out of " << maxSpeed <<
                    "on link " << (*port)->getPortNum() << " of host " << host->getName() << std::endl;
                }
            }
        }
        if (hasErrors) {
            output->failWarning(str.str());
            isOK = false;
        }
    }

    output->printPerformanceData(me);
    output->finish();

    return isOK;
}

