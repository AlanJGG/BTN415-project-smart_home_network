// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
#include <iostream>
#include "../utilities/SocketSystem.h"
#include "TCPServer.h"

int main()
{
    SocketSystem socketSystem;

    TCPServer server(8080);
    server.start();

    return 0;
}