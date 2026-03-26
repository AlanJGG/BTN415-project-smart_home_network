#ifndef DEVICE_H
#define DEVICE_H

#include <string>

// Device Class: Represents smart home devices (lighting, thermostat, security, cameras)
class Device
{
private:
    std::string name;
    std::string ip;
    std::string mac;

public:
    Device(std::string n, std::string i, std::string m);

    std::string getIP() const;
    std::string getMAC() const;
    std::string getName() const;
};

#endif
