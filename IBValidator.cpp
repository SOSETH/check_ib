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
#include "IBValidator.h"
#include "IBHost.h"
#include "IBPort.h"

namespace check_ib {

    const bool IBValidator::hasLinkTo(const std::shared_ptr<IBHost> src, const std::shared_ptr<IBHost> dest)
            const noexcept {
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
}
