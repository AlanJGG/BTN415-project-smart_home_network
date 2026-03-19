//   We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
#include <iostream>
#include <string>
#include "../utilities/socketutils.h"
#include "../utilities/socketsystem.h"


using namespace std;

//protocol menu
void displayMenu(){
   cout << "\n ---- AVAILABLE COMMANDS ----" << endl;
   cout << "GET /light/on - Turn light ON" << endl;
   cout << "GET /light/off - Turn light OFF" << endl;
   cout << "GET /light/status - Check light status" << endl;
   cout << "GET /thermostat/set/22 - Set temperature (any num)" << endl;
   cout << "GET /thermostat/status - Check thermostat" << endl;
   cout << "GET /camera/on - Turn camera ON" << endl;
   cout << "GET /camera/off - Turn camera OFF" << endl;
   cout << "GET /camera/start - Start recording" << endl;
   cout << "GET /camera/stop - Stop recording" << endl;
   cout << "GET /camera/status - Check camera status" << endl;
   cout << "\n ---- Type 'q' to quit ----" << endl;
   cout << "==============================\n" << endl;

}
int main() {
   SocketSystem ss;

    socket_t clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) { //managing connection failure
        cout << "connection failed\n";
        return 1;
    }

    cout << "connected to server\n";
    
    displayMenu();
    while (true) {
        string command;
        getline(cin, command);
        
        if (command.empty()){
            cout << "Please type a command or 'q' to quit." << endl;
            continue;
        }
        if (command == "q" || command == "Q"){
            cout << "Disconnecting..." << endl;
            break;
        }
        send(clientSocket, command.c_str(), command.size(), 0);

        char buffer[1024];
        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

        cout << "server response: " << string(buffer, bytes) << endl;
    }
    close_socket(clientSocket);
    return 0;
}
