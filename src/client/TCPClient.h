#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <string>
#include "../utilities/SocketUtils.h"

class TCPClient
{
private:
    std::string m_serverIp;
    int m_port;
    socket_t m_clientSocket;

    bool connectToServer();
    void displayMenu() const;

public:
    TCPClient(const std::string &serverIp = "127.0.0.1", int port = 8080);
    ~TCPClient();

    void run();
};

#endif