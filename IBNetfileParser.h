//
// Created by maxf on 22.06.16.
//

#ifndef CHECK_IB_IBNETFILEPARSER_H
#define CHECK_IB_IBNETFILEPARSER_H

#include <string>
#include "IBException.h"

class IBNetfileParser {
public:
    class IBNetfileParserException : public virtual IBException {
    public:
        IBNetfileParserException(const std::string &arg) : IBException(arg), domain_error(arg) { }
        IBNetfileParserException(const char *arg) : IBException(arg), domain_error(arg) { }
    };

    IBNetfileParser(std::string & src) throw(IBNetfileParserException);
    IBNetfileParser(std::string && src) throw(IBNetfileParserException);

    std::string getNodeName(const uint64_t guid) const;

private:
    std::map<uint64_t, std::string> nodeNames;
    std::string file, defaultNodeName;
    void initialize() throw(IBNetfileParserException);
};


#endif //CHECK_IB_IBNETFILEPARSER_H
