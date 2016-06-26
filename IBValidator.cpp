//
// Created by maxf on 23.06.16.
//

#include "IBValidator.h"
#include "IBHost.h"
#include "IBPort.h"

IBValidator::IBValidator(std::shared_ptr<IBNetfileParser> parser, std::shared_ptr<IcingaOutput> output) :
        parser(parser), output(output) {}

const bool IBValidator::hasLinkTo(const std::shared_ptr<IBHost> src, const std::shared_ptr<IBHost> dest) const {
    auto ports = src->getPorts();
    for (auto port = ports.begin(); port != ports.end(); port++) {
        if (auto peer = (*port)->getPeer().lock()) {
            if (auto host = peer->getHost().lock()) {
                if (host == dest) {
                    return true;
                }
            }
        }
    }
    return false;
}