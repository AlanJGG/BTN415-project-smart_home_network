#ifndef ARPTABLE_H
#define ARPTABLE_H

#include <string>
#include <map>

// ARPTable: Maps IP addresses -> MAC addresses (simulates ARP protocol)
class ARPTable {
public:
    std::map<std::string, std::string> table;

    void addEntry(const std::string& ip, const std::string& mac);
    std::string resolve(const std::string& ip);  // returns MAC or "ARP_MISS"
};

#endif
