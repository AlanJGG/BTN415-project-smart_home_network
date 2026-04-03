//
//  Subnet.h
//  NetworkSimulator
//
// 
//

#ifndef SUBNET_H
#define SUBNET_H

#include <string>
#include "Device.h"
#include <vector>

//Subnet Class: Handles subnetting

class Subnet {
public:
    std::string networkAddress;
    std::string subnetMask;
    std::string gateway;
    std::vector<Device> devices;
    
    Subnet(std::string net, std::string mask, std::string gw);
    
    void addDevice(Device d);
    std::string getGateway() { return gateway; }
};

#endif
