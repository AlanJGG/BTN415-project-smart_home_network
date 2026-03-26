#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include "Device.h"
#include "ARPTable.h"
#include "Router.h"
#include <string>

// For simulating communication
std::string sendPacket(const Device &sender,
                       const std::string &targetIP,
                       ARPTable &arp,
                       const Router &router);

#endif
