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
