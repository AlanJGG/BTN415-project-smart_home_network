#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include "Device.h"
#include "ARPTable.h"
#include "Router.h"
#include <string>

// Simulates sending a network packet: ARP resolution + routing lookup.
// Returns a human-readable trace string (logged server-side and sent to client).
std::string sendPacket(const Device& sender,
                       const std::string& targetIP,
                       ARPTable& arp,
                       const Router& router);

#endif
