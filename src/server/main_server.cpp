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