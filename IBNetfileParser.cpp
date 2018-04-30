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

#include <yaml-cpp/yaml.h>
#include "IBNetfileParser.h"
#include "IBHost.h"
#include "IBHostRegistry.h"
#include <iostream>

namespace check_ib {

    IBNetfileParser::IBNetfileParser(std::string &src) throw(IBNetfileParser::IBNetfileParserException) {
        this->file = src;
        initialize();
    }

    IBNetfileParser::IBNetfileParser(std::string &&src) throw(IBNetfileParser::IBNetfileParserException) {
        this->file = src;
        initialize();
    }

    IBNetfileParser::IBNetfileParser(const std::string &src) throw(IBNetfileParser::IBNetfileParserException) {
        this->file = src;
        initialize();
    }

    void IBNetfileParser::initialize() throw(IBNetfileParser::IBNetfileParserException) {
        defaultNodeName = "UNDEFINED";
        numUnknownNodes = 0;

        netfile = YAML::LoadFile(file);
        if (!netfile["nodes"] || !netfile["nodes"].IsSequence())
            throw IBNetfileParserException("Required section 'nodes' missing or not a sequence!");
        if (!netfile["links"] || !netfile["links"].IsSequence())
            throw IBNetfileParserException("Required section 'nodes' missing or not a sequence!");

        // Parse nodes
        YAML::Node nodes = netfile["nodes"];
        for (auto node = nodes.begin(); node != nodes.end(); node++) {
            if (!(*node)["guid"])
                throw IBNetfileParserException("Required attribute 'guid' of node is missing!");
            if (!(*node)["name"])
                throw IBNetfileParserException("Required attribute 'name' of node is missing!");
            nodeNames[(*node)["guid"].as<uint64_t>()] = (*node)["name"].as<std::string>();
            if ((*node)["isSM"] && (*node)["isSM"].as<bool>())
                subnetManagers.push_back((*node)["guid"].as<uint64_t>());
        }
    }

    std::string IBNetfileParser::getNodeName(const uint64_t guid) noexcept {
        if (nodeNames.count(guid) == 1)
            return nodeNames.at(guid);
        else {
            std::string name = std::string(defaultNodeName).append(std::to_string(numUnknownNodes++));
            nodeNames[guid] = name;
            return name;
        }
    }

    void IBNetfileParser::finishParsing(std::shared_ptr<IBHostRegistry> hostReg, bool throwOnMissingHost)
            throw(IBNetfileParser::IBNetfileParserException) {
        // Parse links
        YAML::Node links = netfile["links"];
        for (auto link = links.begin(); link != links.end(); link++) {
            if (!(*link)["from"])
                throw IBNetfileParserException("Required attribute 'from' of link is missing!");
            if (!(*link)["to"])
                throw IBNetfileParserException("Required attribute 'to' of link is missing!");
            std::string src = (*link)["from"].as<std::string>();
            std::string dest = (*link)["to"].as<std::string>();
            if (!hostReg->has(src)) {
                if (throwOnMissingHost)
                    throw IBNetfileParserException("Coudln't resolve source of link - host does not exist!");
                else
                    continue;
            }
            if (!hostReg->has(dest)) {
                if (throwOnMissingHost)
                    throw IBNetfileParserException("Coudln't resolve destination of link - host does not exist!");
                else
                    continue;
            }
            if (src == dest) {
                if (throwOnMissingHost)
                    throw IBNetfileParserException("Loopback links are not supported!");
                else
                    continue;
            }
            expectedLinks.push_back(std::make_pair((*hostReg)[src], (*hostReg)[dest]));
        }
    }

    std::list<std::pair<std::shared_ptr<IBHost>, std::shared_ptr<IBHost>>> IBNetfileParser::getExpectedLinks()
            const noexcept {
        return expectedLinks;
    }

    std::list<uint64_t> IBNetfileParser::getSubnetManagers() const noexcept {
        return subnetManagers;
    }
}
