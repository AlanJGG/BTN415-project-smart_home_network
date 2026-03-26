#include "Device.h"

Device::Device(std::string n, std::string i, std::string m)
    : name(n), ip(i), mac(m) {}

std::string Device::getIP()   const { return ip; }
std::string Device::getMAC()  const { return mac; }
std::string Device::getName() const { return name; }
