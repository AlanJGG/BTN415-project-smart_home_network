//
//  Subnet.cpp
//  NetworkSimulator
//
// 
//

#include "Subnet.h"

 Subnet::Subnet(std::string net, std::string mask, std::string gw)
    : networkAddress(net), subnetMask(mask), gateway(gw){}

void Subnet::addDevice(Device d) {
    devices.push_back(d);
}

