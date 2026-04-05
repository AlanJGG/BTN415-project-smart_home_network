// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05
#include "TCPClient.h"
#include "../utilities/SocketSystem.h"
#include <iostream>

int main(int argc, char *argv[])
{
    SocketSystem sys;

    std::string ip = (argc > 1) ? argv[1] : "127.0.0.1";
    int port = (argc > 2) ? std::stoi(argv[2]) : 8080;

    TCPClient client(ip, port);
    client.run();
    
    return 0;
}
