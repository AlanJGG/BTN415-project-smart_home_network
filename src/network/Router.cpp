//
//  Router.cpp
//  NetworkSimulator
//
// 
//

#include "Router.h"

std::string Router::getGateway(std::string destIP){
    int ipLast = std::stoi(destIP.substr(destIP.find_last_of('.')+ 1));

    for (auto& route: routingTable){
        int base = std::stoi(route.first.substr(route.first.find_last_of('.')+ 1));
        
        if (ipLast >= base && ipLast < base + 64){
            return route.second;
        }
    }
    return "No gateway found";
}
void Router::addRoute(std::string destination, std::string nextHop) {
        routingTable[destination] = nextHop;
    }


void Router::routePacket(std::string destIP) {
    
    int ipLast = std::stoi(destIP.substr(destIP.find_last_of('.') + 1));
    //Example: destIP = 192.168.1.70 -> lastOctet = 70
    
    for (auto& route : routingTable) {
        
        int base = std::stoi(route.first.substr(route.first.find_last_of('.') + 1));
        //Example: route = 192.168.1.64 -> base = 64
        
        if (ipLast >= base && ipLast < base + 64) {
            /* Example result:
             70 >= 64
             70 < 128 (64 + 64)
             Output: Routing to Thermostat Gateway
             
             /26 subnet mask if for 26 bits to be used for the network portion of the IP address,
             which leaves 6 bits for host addresses and results 64 total addresses and 62
             usable hosts per subnet.
             */
            std::cout << "Routing to " << route.second << std::endl;
            return;
        }
    }
    std::cout << "No route found\n";
}
