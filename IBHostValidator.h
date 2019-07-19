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

#ifndef CHECK_IB_IBHOSTVALIDATOR_H
#define CHECK_IB_IBHOSTVALIDATOR_H

#include "IBValidator.h"

namespace check_ib {

    class IBHostValidator : public IBValidator {
    private:
        std::string myHostName;

    public:
        IBHostValidator(const std::string &) noexcept;

        IBHostValidator(const std::string &&) noexcept;

        virtual bool isValid(std::shared_ptr<IBNetfileParser>, std::shared_ptr<IcingaOutput>,
                             std::shared_ptr<IBHostRegistry>);
    };
}

#endif //CHECK_IB_IBHOSTVALIDATOR_H
