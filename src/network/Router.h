//
//  Router.h
//  NetworkSimulator
//
//
//

#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <map>
#include <iostream>

//Router class: Router + Static Routing

class Router {
public:
    std::map<std::string, std::string> routingTable;
    
    void addRoute(std::string destination, std::string nextHop);
    
    std::string routePacket(std::string destIP);

    std::string getGateway(std::string destIP);
};
#endif
