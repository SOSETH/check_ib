//
// Created by maxf on 22.06.16.
//

#include <yaml-cpp/yaml.h>
#include "IBNetfileParser.h"
#include "IBHost.h"
#include "IBHostRegistry.h"
#include <iostream>

IBNetfileParser::IBNetfileParser(std::string & src) throw(IBNetfileParser::IBNetfileParserException) {
    this->file = src;
    initialize();
}

IBNetfileParser::IBNetfileParser(std::string && src) throw(IBNetfileParser::IBNetfileParserException) {
    this->file = src;
    initialize();
}

IBNetfileParser::IBNetfileParser(const std::string& src) throw(IBNetfileParser::IBNetfileParserException) {
    this->file = src;
    initialize();
}

void IBNetfileParser::initialize() throw(IBNetfileParser::IBNetfileParserException) {
    defaultNodeName = "UNDEFINED";

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

std::string IBNetfileParser::getNodeName(const uint64_t guid) const {
    if (nodeNames.count(guid) == 1)
        return nodeNames.at(guid);
    else {
        return defaultNodeName;
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

std::list<std::pair<std::shared_ptr<IBHost>, std::shared_ptr<IBHost>>> IBNetfileParser::getExpectedLinks() const {
    return expectedLinks;
}

std::list<uint64_t> IBNetfileParser::getSubnetManagers() const {
    return subnetManagers;
}
