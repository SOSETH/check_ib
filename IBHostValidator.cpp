//
// Created by maxf on 23.06.16.
//

#include "IBHostValidator.h"
#include "IBHostRegistry.h"
#include "IBNetfileParser.h"
#include "IBHost.h"
#include "IBPort.h"
#include "IcingaOutput.h"

IBHostValidator::IBHostValidator(const std::string& hostname) : myHostName(hostname) {}
IBHostValidator::IBHostValidator(const std::string&& hostname) : myHostName(hostname) {}

bool IBHostValidator::isValid(std::shared_ptr<IBHostRegistry> registry, std::shared_ptr<IBNetfileParser> parser,
                              std::shared_ptr<IcingaOutput> output) {
    bool isOK=true;
    if (!registry->has(myHostName)) {
        output->failCritical("Coudln't find host to check!");
        return false;
    }
    auto me = (*registry)[myHostName];
    output->setIBHostDetail(me);

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

    output->printPerformanceData(me);
    output->finish();
    return isOK;
}

