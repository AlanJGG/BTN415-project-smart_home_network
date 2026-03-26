#include "Subnet.h"

Subnet::Subnet(const std::string& net, const std::string& mask)
    : networkAddress(net), subnetMask(mask) {}

void Subnet::addDevice(const Device& d) {
    devices.push_back(d);
}
