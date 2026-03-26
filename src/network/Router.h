#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <map>
#include <iostream>

// Router: Simulates static routing between subnets (/26 blocks)
class Router {
public:
    std::map<std::string, std::string> routingTable; // subnet base -> gateway label

    void addRoute(const std::string& destination, const std::string& nextHop);

    // Returns the gateway name for destIP, or "NO_ROUTE"
    std::string routePacket(const std::string& destIP) const;
};

#endif
