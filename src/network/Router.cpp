#include "Router.h"

void Router::addRoute(const std::string &destination, const std::string &nextHop)
{
    routingTable[destination] = nextHop;
}

std::string Router::routePacket(const std::string &destIP) const
{
    // Extract last octet of destination IP
    int lastOctet = std::stoi(destIP.substr(destIP.find_last_of('.') + 1));
    // Example: destIP = 192.168.1.70 -> lastOctet = 70
    for (const auto &route : routingTable)
    {
        int base = std::stoi(route.first.substr(route.first.find_last_of('.') + 1));
        // Example: route = 192.168.1.64 -> base = 64
        if (lastOctet >= base && lastOctet < base + 64)
        {
            /* Example result:
             70 >= 64
             70 < 128 (64 + 64)
             Output: Routing to Thermostat Gateway

             /26 subnet mask if for 26 bits to be used for the network portion of the IP address,
             which leaves 6 bits for host addresses and results 64 total addresses and 62
             usable hosts per subnet.
             */
            return route.second;
        }
    }
    return "NO_ROUTE";
}
