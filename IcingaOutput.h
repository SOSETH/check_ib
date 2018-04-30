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

#ifndef CHECK_IB_ICINGAOUTPUT_H
#define CHECK_IB_ICINGAOUTPUT_H

#include <string>
#include <memory>
#include <map>
#include <sstream>

namespace check_ib {
    class IBHost;
    class IBSubnetManager;

    class IcingaOutput {
    public:
        std::ostream &failCritical() noexcept;
        std::ostream &failWarning() noexcept;
        std::ostream &failUnknown() noexcept;

        void setIBHostDetail(std::shared_ptr<IBHost>) noexcept;
        void setIBSubnetManagersDetail(std::shared_ptr<std::map<uint64_t, std::shared_ptr<IBSubnetManager>>>) noexcept;

        void finish() noexcept;

        IcingaOutput() noexcept;

        const int getRC() const noexcept {
            return rc;
        }

        void printPerformanceData(std::shared_ptr<IBHost>) noexcept;
        void printPerformanceData(std::shared_ptr<std::map<uint64_t, std::shared_ptr<IBSubnetManager>>>) noexcept;

        const bool getDidFinish() const noexcept {
            return didFinish;
        }

    private:
        std::function<void()> dumpFun;
        int rc;
        bool isFirstPerformanceMetric = true;
        bool didFinish = false;
        std::ostringstream outputMessage;
        std::ostringstream performanceData;
    };
}

#endif //CHECK_IB_ICINGAOUTPUT_H
