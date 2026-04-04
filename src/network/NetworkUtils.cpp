//
//  NetworkUtils.cpp
//  NetworkSimulator
//
// 
//

#include "NetworkUtils.h"

std::string sendPacket(Device& sender, std::string targetIP, ARPTable& arp, Router& router) {
    std::string result = sender.getName() + " sending packet to " + targetIP + "\n";
    
    std::string mac = arp.resolve(targetIP);
    
    if (mac == "Not found") {
        result += "ARP Request broadcast...\n";
    } else {
       result += "MAC Found: " + mac + "\n";
    }
    std::string gateway = router.routePacket(targetIP);
    result += "Routing to " + gateway + "\n";
    return result;
}
