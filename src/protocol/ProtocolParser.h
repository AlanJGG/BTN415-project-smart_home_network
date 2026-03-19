#ifndef PROTOCOL_PARSER_H
#define PROTOCOL_PARSER_H

#include <string>

struct Request
{
    std::string method;
    std::string device;
    std::string action;
    std::string value;
    bool valid;
    std::string errorMessage;
};

struct Response
{
    std::string statusCode;
    std::string body;

    std::string toString() const;
};

class ProtocolParser
{
public:
    static Request parseRequest(const std::string &rawRequest);
};

#endif
