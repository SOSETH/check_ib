//
// Created by maxf on 22.06.16.
//

#include <yaml-cpp/yaml.h>
#include "IBNetfileParser.h"

IBNetfileParser::IBNetfileParser(std::string & src) throw(IBNetfileParser::IBNetfileParserException) {
    this->file = src;
    initialize();
}

IBNetfileParser::IBNetfileParser(std::string && src) throw(IBNetfileParser::IBNetfileParserException) {
    this->file = src;
    initialize();
}

void IBNetfileParser::initialize() throw(IBNetfileParser::IBNetfileParserException) {
    defaultNodeName = "UNDEFINED";

    YAML::Node netfile = YAML::LoadFile(file);
    if (!netfile["nodes"] || !netfile["nodes"].IsSequence())
        throw IBNetfileParserException("Required section 'nodes' missing or not a sequence!");
    YAML::Node nodes = netfile["nodes"];
    for (auto node = nodes.begin(); node != nodes.end(); node++) {
        if (!(*node)["guid"])
            throw IBNetfileParserException("Required attribute guid of node is missing!");
        if (!(*node)["name"])
            throw IBNetfileParserException("Required attribute name of node is missing!");
        nodeNames[(*node)["guid"].as<uint64_t>()] = (*node)["name"].as<std::string>();
    }
}

std::string IBNetfileParser::getNodeName(const uint64_t guid) const {
    if (nodeNames.count(guid) == 1)
        return nodeNames.at(guid);
    else {
        return defaultNodeName;
    }
}