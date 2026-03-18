#include "SocketSystem.h"
#include <iostream>

SocketSystem::SocketSystem()
{
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        std::cerr << "WSAStartup failed: " << result << std::endl;
    }
    else
    {
        std::cout << "Windows Sockets initialized" << std::endl;
    }
#endif
}

SocketSystem::~SocketSystem()
{
#ifdef _WIN32
    WSACleanup();
#endif
}