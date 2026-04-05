// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05

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
