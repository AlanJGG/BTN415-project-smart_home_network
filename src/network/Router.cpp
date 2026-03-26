#include "Router.h"

void Router::addRoute(const std::string& destination, const std::string& nextHop) {
    routingTable[destination] = nextHop;
}

std::string Router::routePacket(const std::string& destIP) const {
    // Extract last octet of destination IP
    int ipLast = std::stoi(destIP.substr(destIP.find_last_of('.') + 1));

    for (const auto& route : routingTable) {
        int base = std::stoi(route.first.substr(route.first.find_last_of('.') + 1));
        // Each subnet is /26 = 64 addresses
        if (ipLast >= base && ipLast < base + 64) {
            return route.second; // e.g. "Lighting Gateway"
        }
    }
    return "NO_ROUTE";
}
