// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05

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
