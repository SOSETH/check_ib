//
// Created by maxf on 23.06.16.
//

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
