// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05
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
