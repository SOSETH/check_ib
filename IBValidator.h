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

#ifndef CHECK_IB_IBVALIDATOR_H
#define CHECK_IB_IBVALIDATOR_H

#include <memory>
#include "IBException.h"

namespace check_ib {
    class IBHostRegistry;
    class IcingaOutput;
    class IBNetfileParser;
    class IBHost;

    class IBValidator {
    public:
        virtual bool isValid(std::shared_ptr<IBNetfileParser>, std::shared_ptr<IcingaOutput>,
                             std::shared_ptr<IBHostRegistry>) = 0;

        const bool hasLinkTo(const std::shared_ptr<IBHost>, const std::shared_ptr<IBHost>) const noexcept;
    };
}

#endif //CHECK_IB_IBVALIDATOR_H
