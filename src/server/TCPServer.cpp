// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects

#include "TCPServer.h"
#include <iostream>
#include <thread>
#include <stdexcept>
#include "../protocol/ProtocolParser.h"

// Light implementation
TCPServer::Light::Light() : m_isOn(false)
{
}

std::string TCPServer::Light::on()
{
    m_isOn = true;
    return "Light is on";
}

std::string TCPServer::Light::off()
{
    m_isOn = false;
    return "Light is off";
}

std::string TCPServer::Light::status() const
{
    return m_isOn ? "Light is on" : "Light is off";
}

// Thermostat implementation
TCPServer::Thermostat::Thermostat() : m_temp(22)
{
}

std::string TCPServer::Thermostat::setTemp(int temp)
{
    m_temp = temp;
    return "Set temperature to: " + std::to_string(m_temp);
}

std::string TCPServer::Thermostat::status() const
{
    return "Current temperature: " + std::to_string(m_temp);
}

// Camera implementation
TCPServer::Camera::Camera() : m_isOn(false), m_recording(false)
{
}

std::string TCPServer::Camera::on()
{
    m_isOn = true;
    return "Camera is on";
}

std::string TCPServer::Camera::off()
{
    m_isOn = false;
    m_recording = false;
    return "Camera is off";
}

std::string TCPServer::Camera::start()
{
    if (!m_isOn)
    {
        return "Cannot record because Camera is not turned on. Use GET /camera/on first.";
    }

    m_recording = true;
    return "Recording...";
}

std::string TCPServer::Camera::stop()
{
    m_recording = false;
    return "Recording stopped";
}

std::string TCPServer::Camera::status() const
{
    return std::string("Camera: ") +
           (m_isOn ? "ON" : "OFF") + ", " +
           (m_recording ? "Recording" : "Not Recording");
}

// TCPServer constructor
TCPServer::TCPServer(int port)
    : m_port(port), m_serverSocket(INVALID_SOC)
{
}

TCPServer::~TCPServer()
{
    if (m_serverSocket != INVALID_SOC)
    {
        close_socket(m_serverSocket);
    }
}

// Request processing
std::string TCPServer::processRequest(const std::string &rawRequest)
{
    Request req = ProtocolParser::parseRequest(rawRequest);
    Response res;

    if (!req.valid)
    {
        res.statusCode = "400 Bad Request";
        res.body = req.errorMessage;
        return res.toString();
    }

    if (req.device == "light")
    {
        if (req.action == "on")
        {
            res.statusCode = "200 OK";
            res.body = m_light.on();
        }
        else if (req.action == "off")
        {
            res.statusCode = "200 OK";
            res.body = m_light.off();
        }
        else if (req.action == "status")
        {
            res.statusCode = "200 OK";
            res.body = m_light.status();
        }
        else
        {
            res.statusCode = "404 Not Found";
            res.body = "Invalid light action";
        }
    }
    else if (req.device == "thermostat")
    {
        if (req.action == "set")
        {
            try
            {
                int temperature = stoi(req.value);
                res.statusCode = "200 OK";
                res.body = m_thermostat.setTemp(temperature);
            }
            catch (...)
            {
                res.statusCode = "400 Bad Request";
                res.body = "Invalid temperature value";
            }
        }
        else if (req.action == "status")
        {
            res.statusCode = "200 OK";
            res.body = m_thermostat.status();
        }
        else
        {
            res.statusCode = "404 Not Found";
            res.body = "Invalid thermostat action";
        }
    }
    else if (req.device == "camera")
    {
        if (req.action == "on")
        {
            res.statusCode = "200 OK";
            res.body = m_camera.on();
        }
        else if (req.action == "off")
        {
            res.statusCode = "200 OK";
            res.body = m_camera.off();
        }
        else if (req.action == "start")
        {
            res.statusCode = "200 OK";
            res.body = m_camera.start();
        }
        else if (req.action == "stop")
        {
            res.statusCode = "200 OK";
            res.body = m_camera.stop();
        }
        else if (req.action == "status")
        {
            res.statusCode = "200 OK";
            res.body = m_camera.status();
        }
        else
        {
            res.statusCode = "404 Not Found";
            res.body = "Invalid camera action";
        }
    }
    else
    {
        res.statusCode = "404 Not Found";
        res.body = "Unknown device";
    }

    return res.toString();
}

// Client handler
void TCPServer::handleClient(socket_t clientSocket)
{
    char buffer[1024];

    while (true)
    {
        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytes <= 0)
        {
            break;
        }

        std::string request(buffer, bytes);
        std::string response;

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            response = processRequest(request);
        }

        send(clientSocket, response.c_str(), static_cast<int>(response.size()), 0);
    }

    close_socket(clientSocket);
    std::cout << "Client disconnected" << std::endl;
}

// Server start
void TCPServer::start()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == INVALID_SOC)
    {
        report_error("Failed to create server socket");
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        report_error("Bind failed");
        close_socket(m_serverSocket);
        m_serverSocket = INVALID_SOC;
        return;
    }

    if (listen(m_serverSocket, 5) < 0)
    {
        report_error("Listen failed");
        close_socket(m_serverSocket);
        m_serverSocket = INVALID_SOC;
        return;
    }

    std::cout << "Server running on port " << m_port << "..." << std::endl;

    while (true)
    {
        socket_t clientSocket = accept(m_serverSocket, nullptr, nullptr);

        if (clientSocket == INVALID_SOC)
        {
            report_error("Accept failed");
            continue;
        }

        std::cout << "Client connected" << std::endl;

        std::thread clientThread(&TCPServer::handleClient, this, clientSocket);
        clientThread.detach();
    }
}