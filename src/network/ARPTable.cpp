// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05

#include "ARPTable.h"

void ARPTable::addEntry(const std::string &ip, const std::string &mac)
{
    table[ip] = mac;
}

std::string ARPTable::resolve(const std::string &ip)
{
    auto it = table.find(ip);
    if (it != table.end())
        return it->second;
    return "ARP_MISS";
}
