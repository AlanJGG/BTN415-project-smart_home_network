//   We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include "../utilities/socketutils.h"
#include "../utilities/socketsystem.h"


using namespace std;
mutex mtx; 

//devices
class Light {
    bool isOn;
public:
    Light(){
        isOn = false;
    }

    string on() { isOn = true; return "Light is on"; }
    string off() { isOn = false; return "Light is off"; }
    string status() { return isOn ? "Light is on" : "Light is off"; }
};

class Thermostat {
    int temp;
public:
    Thermostat(){
        temp= 22;
    }

    string setTemp(int t) {
        temp = t;
        return "Set temperature to: " + to_string(temp);
    }

    string status() {
        return "Current temperature:" + to_string(temp);
    }
};

class Camera {
    bool isOn;
    bool recording;
public:
    Camera(){
        isOn = false;
        recording = false;
    }

    string on() { isOn = true; return "Camera is on"; }
    string off() { isOn = false; recording = false; return "Camera is off"; }

    string start() {
        if (!isOn){
            return "Camera is off.";
        }
        recording = true;
        return "Recording...";
    }

    string stop() {
        recording = false;
        return "Recording stopped";
    }

    string status() {
        return string("Camera: ") +
               (isOn ? "ON" : "OFF") + ", " +
               (recording ? "Recording" : "Not Recording");
    }
};

//setting global devices
Light light;
Thermostat thermostat;
Camera camera;

//managing processing
string processRequest(string req) {
    //light
    string responseCode = "200 OK";
    string result;
     
    if (req == "GET /light/on") {result = light.on();}
    else if (req == "GET /light/off") {result = light.off();}
    else if (req == "GET /light/status") {result = light.status();}
    
    //thermostat
    else if (req.find("GET /thermostat/set/") == 0) {
        int value = stoi(req.substr(22));
        result = thermostat.setTemp(value);
    }
    else if (req == "GET /thermostat/status") {result = thermostat.status();}

    //camera
    else if (req == "GET /camera/on") {result = camera.on();}
    else if (req == "GET /camera/off") {result = camera.off();}
    else if (req == "GET /camera/start") {result = camera.start();}
    else if (req == "GET /camera/stop") {result = camera.stop();}
    else if (req == "GET /camera/status") {result = camera.status();}
    else{
        responseCode = "404 Not Found";
        result = "invalid request";
    }
    return responseCode + " - " + result;
}

void handleClient(socket_t clientSocket) {
    char buffer[1024]; //stores incoming data

    while (true) {
        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;

        string request(buffer, bytes);

        //lock to prevent race conditions
        mtx.lock();
        string response = processRequest(request);
        mtx.unlock();

        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close_socket(clientSocket);
    cout << "client disconnected" << endl;
}



//main program
int main() {
  
    SocketSystem ss;
    socket_t serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    cout << "server running on port 8080..." <<endl;

    while (true) {
        socket_t clientSocket = accept(serverSocket, NULL, NULL);
        cout << "client connected" << endl;

        thread t(handleClient, clientSocket);
        t.detach();  // allows multiple clients

        
    }

   
    return 0;
}
