// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
#include "TCPClient.h"
#include <iostream>

TCPClient::TCPClient(const std::string &serverIp, int port)
    : m_serverIp(serverIp), m_port(port), m_clientSocket(INVALID_SOC)
{
}

TCPClient::~TCPClient()
{
    if (m_clientSocket != INVALID_SOC)
    {
        close_socket(m_clientSocket);
    }
}

void TCPClient::displayMenu() const
{
    std::cout << "\n---- AVAILABLE COMMANDS ----" << std::endl;
    std::cout << "GET /light/on - Turn light ON" << std::endl;
    std::cout << "GET /light/off - Turn light OFF" << std::endl;
    std::cout << "GET /light/status - Check light status" << std::endl;
    std::cout << "GET /thermostat/set/22 - Set temperature (any number)" << std::endl;
    std::cout << "GET /thermostat/status - Check thermostat" << std::endl;
    std::cout << "GET /camera/on - Turn camera ON" << std::endl;
    std::cout << "GET /camera/off - Turn camera OFF" << std::endl;
    std::cout << "GET /camera/start - Start recording" << std::endl;
    std::cout << "GET /camera/stop - Stop recording" << std::endl;
    std::cout << "GET /camera/status - Check camera status" << std::endl;
    std::cout << "\nType 'q' to quit" << std::endl;
    std::cout << "============================\n"
              << std::endl;
}

bool TCPClient::connectToServer()
{
    m_clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_clientSocket == INVALID_SOC)
    {
        report_error("Failed to create client socket");
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_port);

#ifdef _WIN32
    serverAddr.sin_addr.s_addr = inet_addr(m_serverIp.c_str());
#else
    if (inet_pton(AF_INET, m_serverIp.c_str(), &serverAddr.sin_addr) <= 0)
    {
        report_error("Invalid server IP address");
        close_socket(m_clientSocket);
        m_clientSocket = INVALID_SOC;
        return false;
    }
#endif

    if (::connect(m_clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        report_error("Connection failed");
        close_socket(m_clientSocket);
        m_clientSocket = INVALID_SOC;
        return false;
    }

    std::cout << "[CLIENT] Connected to " << m_serverIp << ":" << m_port << std::endl;
    return true;
}

void TCPClient::run()
{
    if (!connectToServer())
        return;

    displayMenu();

    while (true)
    {
        std::cout << "> ";
        std::string command;
        std::getline(std::cin, command);

        if (command.empty())
        {
            std::cout << "Please type a command or 'q' to quit." << std::endl;
            continue;
        }

        if (command == "q" || command == "Q")
        {
            std::cout << "[CLIENT] Disconnecting..." << std::endl;
            break;
        }

        int sentBytes = ::send(m_clientSocket, command.c_str(),
                               static_cast<int>(command.size()), 0);

        if (sentBytes < 0)
        {
            report_error("Send failed");
            break;
        }

        char buffer[1024]{};
        int receivedBytes = ::recv(m_clientSocket, buffer, sizeof(buffer), 0);

        if (receivedBytes <= 0)
        {
            std::cout << "Server disconnected or receive failed." << std::endl;
            break;
        }

        std::cout << "Server response: " << std::string(buffer, receivedBytes) << std::endl;
    }
}