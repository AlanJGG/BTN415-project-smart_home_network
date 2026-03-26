#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include "../utilities/SocketUtils.h"
#include "../network/Device.h"
#include "../network/ARPTable.h"
#include "../network/Router.h"
#include "../network/Subnet.h"
#include "../network/NetworkUtils.h"
#include "../protocol/ProtocolParser.h"

class TCPServer
{
private:
    // Socket state
    int m_port;
    socket_t m_serverSocket;

    // Network Layer
    Device m_lightDevice;
    Device m_thermoDevice;
    Device m_cameraDevice;

    Subnet m_lightingSubnet;
    Subnet m_thermostatSubnet;
    Subnet m_cameraSubnet;

    ARPTable m_arp;
    Router m_router;

    // Device state
    struct Light
    {
        bool isOn = false;
        std::mutex mtx;
        std::string on();
        std::string off();
        std::string status();
    };

    struct Thermostat
    {
        int temp = 22;
        std::mutex mtx;
        std::string setTemp(int t);
        std::string status();
    };

    struct Camera
    {
        bool isOn = false;
        bool recording = false;
        std::mutex mtx;
        std::string on();
        std::string off();
        std::string start();
        std::string stop();
        std::string status();
    };

    Light m_light;
    Thermostat m_thermostat;
    Camera m_camera;

    // Command queues per devices
    struct DeviceCommand
    {
        Request req;
        socket_t clientSock;
        std::string networkTrace;
    };

    std::queue<DeviceCommand> m_lightQueue;
    std::queue<DeviceCommand> m_thermoQueue;
    std::queue<DeviceCommand> m_cameraQueue;

    std::mutex m_lightMtx, m_thermoMtx, m_cameraMtx;
    std::condition_variable m_lightCV, m_thermoCV, m_cameraCV;
    bool m_running = true;

    // Worker threads one per device
    std::thread m_lightThread;
    std::thread m_thermoThread;
    std::thread m_cameraThread;

    void lightWorker();
    void thermoWorker();
    void cameraWorker();

    // Client Handler
    void handleClient(socket_t clientSocket);
    void dispatchRequest(const Request &req, socket_t clientSock);

public:
    TCPServer(int port = 8080);
    ~TCPServer();
    void start();
};

#endif