// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05

#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>
#include <iostream>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define INVALID_SOC INVALID_SOCKET
#else
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#define INVALID_SOC -1
#endif

#ifdef _WIN32
using socket_t = SOCKET;
using socklen_t = int;
#else
using socket_t = int;
#endif

inline void close_socket(socket_t s)
{
#ifdef _WIN32
    closesocket(s);
#else
    close(s);
#endif
}

inline void report_error(const std::string &error_msg)
{
#ifdef _WIN32
    std::cerr << error_msg << ": " << WSAGetLastError() << std::endl;
    WSACleanup();
#else
    std::cerr << error_msg << ": " << strerror(errno) << std::endl;
#endif
}

#endif
