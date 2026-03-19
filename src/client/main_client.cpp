// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
#include "../utilities/SocketSystem.h"
#include "TCPClient.h"

int main()
{
    SocketSystem socketSystem;

    TCPClient client("127.0.0.1", 8080);
    client.run();

    return 0;
}