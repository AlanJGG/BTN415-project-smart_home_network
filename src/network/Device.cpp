// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05

#include "Device.h"

Device::Device(std::string n, std::string i, std::string m)
    : name(n), ip(i), mac(m) {}

std::string Device::getIP() const { return ip; }
std::string Device::getMAC() const { return mac; }
std::string Device::getName() const { return name; }
