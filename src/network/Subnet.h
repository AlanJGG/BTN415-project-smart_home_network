#ifndef SUBNET_H
#define SUBNET_H

#include <string>
#include <vector>
#include "Device.h"

// Subnet: Groups devices under a network address / mask (VLSM /26)
class Subnet {
public:
    std::string networkAddress;
    std::string subnetMask;
    std::vector<Device> devices;

    Subnet(const std::string& net, const std::string& mask);
    void addDevice(const Device& d);
};

#endif
