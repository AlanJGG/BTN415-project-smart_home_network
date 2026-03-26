// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
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