//
// Created by maxf on 23.06.16.
//

#ifndef CHECK_IB_ICINGAOUTPUT_H
#define CHECK_IB_ICINGAOUTPUT_H

#include <string>
#include <memory>
#include <map>
#include "IBException.h"

class IBHost;
class IBSubnetManager;

class IcingaOutput {
public:
    class IcingaOutputException : public virtual IBException {
    public:
        IcingaOutputException(const std::string &arg) : IBException(arg), domain_error(arg) { }
        IcingaOutputException(const char *arg) : IBException(arg), domain_error(arg)  { }
    };

    void failCritical(std::string&& reason);
    void failWarning(std::string&& reason);
    void failUnknown(std::string&& reason);
    void setIBHostDetail(std::shared_ptr<IBHost>);
    void setIBSubnetManagersDetail(std::shared_ptr<std::map<uint64_t, std::shared_ptr<IBSubnetManager>>>);
    void finish();
    IcingaOutput();
    const int getRC() const {
        return rc;
    }
    void printPerformanceData(std::shared_ptr<IBHost>);
    void printPerformanceData(std::shared_ptr<std::map<uint64_t, std::shared_ptr<IBSubnetManager>>>);
    const bool getDidFinish() const {
        return didFinish;
    }

private:
    std::function<void()> dumpFun;
    int rc;
    bool isFirstPerformanceMetric = true;
    bool didFinish = false;
};


#endif //CHECK_IB_ICINGAOUTPUT_H
