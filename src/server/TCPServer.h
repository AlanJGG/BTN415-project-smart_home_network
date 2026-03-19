#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>
#include <mutex>
#include "../utilities/SocketUtils.h"

class TCPServer
{
private:
    int m_port;
    socket_t m_serverSocket;
    std::mutex m_mutex;

    class Light
    {
    private:
        bool m_isOn;

    public:
        Light();
        std::string on();
        std::string off();
        std::string status() const;
    };

    class Thermostat
    {
    private:
        int m_temp;

    public:
        Thermostat();
        std::string setTemp(int temp);
        std::string status() const;
    };

    class Camera
    {
    private:
        bool m_isOn;
        bool m_recording;

    public:
        Camera();
        std::string on();
        std::string off();
        std::string start();
        std::string stop();
        std::string status() const;
    };

    Light m_light;
    Thermostat m_thermostat;
    Camera m_camera;

    std::string processRequest(const std::string &request);
    void handleClient(socket_t clientSocket);

public:
    TCPServer(int port = 8080);
    ~TCPServer();

    void start();
};

#endif