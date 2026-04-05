// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05

#ifndef ARPTABLE_H
#define ARPTABLE_H

#include <string>
#include <map>

// ARPTable Class: IP -> MAC mapping
class ARPTable
{
public:
    std::map<std::string, std::string> table;

    void addEntry(const std::string &ip, const std::string &mac);
    std::string resolve(const std::string &ip);
};

#endif
