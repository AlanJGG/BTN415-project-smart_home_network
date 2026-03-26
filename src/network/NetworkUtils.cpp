#include "NetworkUtils.h"
#include <sstream>
#include <iostream>

std::string sendPacket(const Device &sender,
                       const std::string &targetIP,
                       ARPTable &arp,
                       const Router &router)
{
    std::ostringstream trace;
    trace << "[NET] " << sender.getName()
          << " (" << sender.getIP() << ") -> " << targetIP << "\n";

    // ARP resolution
    std::string mac = arp.resolve(targetIP);
    if (mac == "ARP_MISS")
    {
        trace << "[ARP] Broadcast request for " << targetIP
              << " ... no reply (device unknown)\n";
    }
    else
    {
        trace << "[ARP] Resolved " << targetIP << " -> " << mac << "\n";
    }

    // Routing
    std::string gateway = router.routePacket(targetIP);
    if (gateway == "NO_ROUTE")
    {
        trace << "[ROUTE] No route found for " << targetIP << "\n";
    }
    else
    {
        trace << "[ROUTE] Forwarding via " << gateway << "\n";
    }

    // print to server console for demonstration
    std::cout << trace.str();
    return trace.str();
}
