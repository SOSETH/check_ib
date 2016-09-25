//
// Created by maxf on 22.06.16.
//

#ifndef CHECK_IB_IBNETFILEPARSER_H
#define CHECK_IB_IBNETFILEPARSER_H

#include <yaml-cpp/yaml.h>
#include <string>
#include <list>
#include <map>
#include "IBException.h"

namespace check_ib {

    class IBHostRegistry;
    class IBHost;

    class IBNetfileParser {
    public:
        class IBNetfileParserException : public virtual IBException {
        public:
            IBNetfileParserException(const std::string &arg) noexcept : IBException(arg), domain_error(arg) { }
            IBNetfileParserException(const char *arg) noexcept : IBException(arg), domain_error(arg) { }
        };

        IBNetfileParser(std::string &src) throw(IBNetfileParserException);
        IBNetfileParser(std::string &&src) throw(IBNetfileParserException);
        IBNetfileParser(const std::string &src) throw(IBNetfileParserException);

        std::string getNodeName(const uint64_t guid) noexcept;

        void finishParsing(std::shared_ptr<IBHostRegistry>, bool) throw(IBNetfileParserException);

        std::list<std::pair<std::shared_ptr<IBHost>, std::shared_ptr<IBHost>>> getExpectedLinks() const noexcept;

        std::list<uint64_t> getSubnetManagers() const noexcept;

    private:
        std::map<uint64_t, std::string> nodeNames;
        std::list<uint64_t> subnetManagers;
        std::list<std::pair<std::shared_ptr<IBHost>, std::shared_ptr<IBHost>>> expectedLinks;
        std::string file, defaultNodeName;
        YAML::Node netfile;
        unsigned int numUnknownNodes;

        void initialize() throw(IBNetfileParserException);
    };
}


#endif //CHECK_IB_IBNETFILEPARSER_H
