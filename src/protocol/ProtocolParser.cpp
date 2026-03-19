#include "ProtocolParser.h"
#include <sstream>
#include <vector>

std::string Response::toString() const
{
    return statusCode + " - " + body;
}

static std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> parts;
    std::string current;
    std::stringstream ss(str);

    while (getline(ss, current, delimiter))
    {
        if (!current.empty())
        {
            parts.push_back(current);
        }
    }

    return parts;
}

Request ProtocolParser::parseRequest(const std::string &rawRequest)
{
    Request req{};
    req.valid = false;

    std::stringstream ss(rawRequest);
    ss >> req.method;

    std::string path;
    ss >> path;

    if (req.method.empty() || path.empty())
    {
        req.errorMessage = "Malformed request";
        return req;
    }

    if (req.method != "GET")
    {
        req.errorMessage = "Unsupported method";
        return req;
    }

    std::vector<std::string> parts = split(path, '/');

    if (parts.size() < 2)
    {
        req.errorMessage = "Invalid path format";
        return req;
    }

    req.device = parts[0];
    req.action = parts[1];

    if (parts.size() >= 3)
    {
        req.value = parts[2];
    }

    req.valid = true;
    return req;
}