// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05
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
