//
//  NetworkUtils.h
//  NetworkSimulator
//
// 
//

#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include "Device.h"
#include "ARPTable.h"
#include "Router.h"
#include <string>
#include <iostream>

//For simulating communication

std::string sendPacket(Device& sender, std::string targetIP, ARPTable& arp, Router& router);

#endif
