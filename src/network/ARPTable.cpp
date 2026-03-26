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
