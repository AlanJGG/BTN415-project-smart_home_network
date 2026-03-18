#include "TCPServer.h"

#include <iostream>

TCPServer::TCPServer(int port) : port(port), server_sock(INVALID_SOC) {}

TCPServer::~TCPServer()
{
    if (server_sock != INVALID_SOC)
    {
        close_socket(server_sock);
    }
}
// TODO
void TCPServer::start()
{
}