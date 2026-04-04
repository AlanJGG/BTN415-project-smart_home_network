#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

int main(){
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server,5);
    
    std::cout << "Test server on port 8080..." << std::endl;

    SOCKET client = accept(server, NULL, NULL);
    std::cout << "Client connected!" << std::endl;

    char buffer[1024];
    recv(client, buffer, sizeof(buffer), 0);
    std::cout << "Received: " << buffer << std::endl;

    send(client, "200 OK - Test response", 22, 0);

    closesocket(client);
    closesocket(server);
    WSACleanup();

    return 0;
}