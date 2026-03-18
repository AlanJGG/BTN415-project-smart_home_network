#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "../utilities/SocketUtils.h"
#include <cstring>

class TCPServer
{
private:
    int port;
    socket_t server_sock;
public:
    TCPServer(int port);
    ~TCPServer();

    void start();
};

#endif