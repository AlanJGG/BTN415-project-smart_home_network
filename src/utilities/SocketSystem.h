//   We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
#ifndef SOCKET_SYSTEM_H
#define SOCKET_SYSTEM_H

#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

class SocketSystem
{
public:
    SocketSystem();
    ~SocketSystem();
};

#endif
