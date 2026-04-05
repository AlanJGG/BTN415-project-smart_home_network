// We have done all the coding by ourselves and only copied the code that our professor provided to complete our labs and projects
// Group 10 Masoud Khire - 186174231 Jose Manuel Tiznado - 183725233 Ekjot Kaur- 189003239 Alan Joseph Guzman Gutierrez - 177035235   2026-04-05

#include "TCPServer.h"
#include <iostream>
#include <stdexcept>

//  Device method implementations

std::string TCPServer::Light::on()
{
    std::lock_guard<std::mutex> lk(mtx);
    isOn = true;
    return "Light is ON";
}

std::string TCPServer::Light::off()
{
    std::lock_guard<std::mutex> lk(mtx);
    isOn = false;
    return "Light is OFF";
}

std::string TCPServer::Light::status()
{
    std::lock_guard<std::mutex> lk(mtx);
    return isOn ? "Light is ON" : "Light is OFF";
}

std::string TCPServer::Thermostat::setTemp(int t)
{
    std::lock_guard<std::mutex> lk(mtx);
    temp = t;
    return "Temperature set to " + std::to_string(temp) + " C";
}

std::string TCPServer::Thermostat::status()
{
    std::lock_guard<std::mutex> lk(mtx);
    return "Current temperature: " + std::to_string(temp) + " C";
}

std::string TCPServer::Camera::on()
{
    std::lock_guard<std::mutex> lk(mtx);
    isOn = true;
    return "Camera is ON";
}

std::string TCPServer::Camera::off()
{
    std::lock_guard<std::mutex> lk(mtx);
    isOn = false;
    recording = false;
    return "Camera is OFF";
}

std::string TCPServer::Camera::start()
{
    std::lock_guard<std::mutex> lk(mtx);
    if (!isOn)
        return "Error: Camera is OFF. Use GET /camera/on first.";
    recording = true;
    return "Recording started";
}

std::string TCPServer::Camera::stop()
{
    std::lock_guard<std::mutex> lk(mtx);
    recording = false;
    return "Recording stopped";
}
std::string TCPServer::Camera::status()
{
    std::lock_guard<std::mutex> lk(mtx);
    return std::string("Camera: ") + (isOn ? "ON" : "OFF") +
           ", " + (recording ? "Recording" : "Not Recording");
}

//  Constructor that sets up network topology

TCPServer::TCPServer(int port)
    : m_port(port),
      m_serverSocket(INVALID_SOC),
      // Network devices with IPs and MAC addresses
      m_lightDevice("Light1", "192.168.1.10", "AA:BB:CC:DD:01"),
      m_thermoDevice("Thermo1", "192.168.1.70", "AA:BB:CC:DD:02"),
      m_cameraDevice("Camera1", "192.168.1.200", "AA:BB:CC:DD:04"),
      // /26 subnets  (64 addresses each)
      m_lightingSubnet("192.168.1.0", "255.255.255.192", "192.168.1.1"),
      m_thermostatSubnet("192.168.1.64", "255.255.255.192", "192.168.1.65"),
      m_cameraSubnet("192.168.1.192", "255.255.255.192", "192.168.1.193")
{
    // Assign devices to subnets
    m_lightingSubnet.addDevice(m_lightDevice);
    m_thermostatSubnet.addDevice(m_thermoDevice);
    m_cameraSubnet.addDevice(m_cameraDevice);

    // Build ARP table  (IP to MAC)
    m_arp.addEntry("192.168.1.10", "AA:BB:CC:DD:01");
    m_arp.addEntry("192.168.1.70", "AA:BB:CC:DD:02");
    m_arp.addEntry("192.168.1.200", "AA:BB:CC:DD:04");

    // Build static routing table  (subnet base -> gateway label)
    m_router.addRoute("192.168.1.0", "192.168.1.1");
    m_router.addRoute("192.168.1.64", "192.168.1.65");
    m_router.addRoute("192.168.1.192", "192.168.1.193");

    // Start one thread per device
    m_lightThread = std::thread(&TCPServer::lightWorker, this);
    m_thermoThread = std::thread(&TCPServer::thermoWorker, this);
    m_cameraThread = std::thread(&TCPServer::cameraWorker, this);

    std::cout << "[NET] Network topology initialised:\n"
              << "  Lighting subnet   : 192.168.1.0/26   (Light1  @ 192.168.1.10)\n"
              << "  Thermostat subnet : 192.168.1.64/26  (Thermo1 @ 192.168.1.70)\n"
              << "  Camera subnet     : 192.168.1.192/26 (Camera1 @ 192.168.1.200)\n";
}

TCPServer::~TCPServer()
{
    m_running = false;
    m_lightCV.notify_all();
    m_thermoCV.notify_all();
    m_cameraCV.notify_all();
    if (m_lightThread.joinable())
        m_lightThread.join();
    if (m_thermoThread.joinable())
        m_thermoThread.join();
    if (m_cameraThread.joinable())
        m_cameraThread.join();
    if (m_serverSocket != INVALID_SOC)
        close_socket(m_serverSocket);
}

//  Per-device worker threads

void TCPServer::lightWorker()
{
    std::cout << "[THREAD] Light worker started (tid "
              << std::this_thread::get_id() << ")\n";
    while (true)
    {
        std::unique_lock<std::mutex> lk(m_lightMtx);
        m_lightCV.wait(lk, [this]
                       { return !m_lightQueue.empty() || !m_running; });
        if (!m_running && m_lightQueue.empty())
            break;

        DeviceCommand cmd = m_lightQueue.front();
        m_lightQueue.pop();
        lk.unlock();

        Response res;
        res.statusCode = "200 OK";
        if (cmd.req.action == "on")
            res.body = m_light.on();
        else if (cmd.req.action == "off")
            res.body = m_light.off();
        else if (cmd.req.action == "status")
            res.body = m_light.status();
        else
        {
            res.statusCode = "404 Not Found";
            res.body = "Unknown light action";
        }

        std::string reply = cmd.networkTrace + res.toString();
        send(cmd.clientSock, reply.c_str(), static_cast<int>(reply.size()), 0);
    }
    std::cout << "[THREAD] Light worker stopped\n";
}

void TCPServer::thermoWorker()
{
    std::cout << "[THREAD] Thermostat worker started (tid "
              << std::this_thread::get_id() << ")\n";
    while (true)
    {
        std::unique_lock<std::mutex> lk(m_thermoMtx);
        m_thermoCV.wait(lk, [this]
                        { return !m_thermoQueue.empty() || !m_running; });
        if (!m_running && m_thermoQueue.empty())
            break;

        DeviceCommand cmd = m_thermoQueue.front();
        m_thermoQueue.pop();
        lk.unlock();

        Response res;
        res.statusCode = "200 OK";
        if (cmd.req.action == "set")
        {
            try
            {
                int t = std::stoi(cmd.req.value);
                res.body = m_thermostat.setTemp(t);
            }
            catch (...)
            {
                res.statusCode = "400 Bad Request";
                res.body = "Invalid temperature value";
            }
        }
        else if (cmd.req.action == "status")
        {
            res.body = m_thermostat.status();
        }
        else
        {
            res.statusCode = "404 Not Found";
            res.body = "Unknown thermostat action";
        }

        std::string reply = cmd.networkTrace + res.toString();
        send(cmd.clientSock, reply.c_str(), static_cast<int>(reply.size()), 0);
    }
    std::cout << "[THREAD] Thermostat worker stopped\n";
}

void TCPServer::cameraWorker()
{
    std::cout << "[THREAD] Camera worker started (tid "
              << std::this_thread::get_id() << ")\n";
    while (true)
    {
        std::unique_lock<std::mutex> lk(m_cameraMtx);
        m_cameraCV.wait(lk, [this]
                        { return !m_cameraQueue.empty() || !m_running; });
        if (!m_running && m_cameraQueue.empty())
            break;

        DeviceCommand cmd = m_cameraQueue.front();
        m_cameraQueue.pop();
        lk.unlock();

        Response res;
        res.statusCode = "200 OK";
        if (cmd.req.action == "on")
            res.body = m_camera.on();
        else if (cmd.req.action == "off")
            res.body = m_camera.off();
        else if (cmd.req.action == "start")
            res.body = m_camera.start();
        else if (cmd.req.action == "stop")
            res.body = m_camera.stop();
        else if (cmd.req.action == "status")
            res.body = m_camera.status();
        else
        {
            res.statusCode = "404 Not Found";
            res.body = "Unknown camera action";
        }

        std::string reply = cmd.networkTrace + res.toString();
        send(cmd.clientSock, reply.c_str(), static_cast<int>(reply.size()), 0);
    }
    std::cout << "[THREAD] Camera worker stopped\n";
}

//  Request dispatch – parse, ARP+route, enqueue to device thread
void TCPServer::dispatchRequest(const Request &req, socket_t clientSock)
{
    // Resolve what physical device is targeted
    Device *targetDevice = nullptr;
    if (req.device == "light")
        targetDevice = &m_lightDevice;
    else if (req.device == "thermostat")
        targetDevice = &m_thermoDevice;
    else if (req.device == "camera")
        targetDevice = &m_cameraDevice;

    std::string networkTrace;
    if (targetDevice)
    {
        networkTrace = sendPacket(*targetDevice, targetDevice->getIP(),
                                  m_arp, m_router);
    }

    DeviceCommand cmd{req, clientSock, networkTrace};

    if (req.device == "light")
    {
        std::lock_guard<std::mutex> lk(m_lightMtx);
        m_lightQueue.push(cmd);
        m_lightCV.notify_one();
    }
    else if (req.device == "thermostat")
    {
        std::lock_guard<std::mutex> lk(m_thermoMtx);
        m_thermoQueue.push(cmd);
        m_thermoCV.notify_one();
    }
    else if (req.device == "camera")
    {
        std::lock_guard<std::mutex> lk(m_cameraMtx);
        m_cameraQueue.push(cmd);
        m_cameraCV.notify_one();
    }
    else
    {
        // Unknown device – respond immediately
        Response res;
        res.statusCode = "404 Not Found";
        res.body = "Unknown device: " + req.device;
        std::string reply = res.toString();
        send(clientSock, reply.c_str(), static_cast<int>(reply.size()), 0);
    }
}


//  Client handler – runs in its own thread (one thread per conneciton)
void TCPServer::handleClient(socket_t clientSocket)
{
    char buffer[1024];

    while (true)
    {
        int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0)
            break;

        buffer[bytes] = '\0';
        std::string rawRequest(buffer, bytes);

        Request req = ProtocolParser::parseRequest(rawRequest);

        if (!req.valid)
        {
            Response res;
            res.statusCode = "400 Bad Request";
            res.body = req.errorMessage;
            std::string reply = res.toString();
            send(clientSocket, reply.c_str(), static_cast<int>(reply.size()), 0);
        }
        else
        {
            dispatchRequest(req, clientSocket);
        }
    }

    close_socket(clientSocket);
    std::cout << "[SERVER] Client disconnected\n";
}

//  Server start: accept loop

void TCPServer::start()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == INVALID_SOC)
    {
        report_error("Failed to create server socket");
        return;
    }

    // Allow immediate reuse of port after restart
    int opt = 1;
#ifdef _WIN32
    setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<char *>(&opt), sizeof(opt));
#else
    setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
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

    std::cout << "[SERVER] Listening on port " << m_port << " ...\n";

    while (true)
    {
        socket_t clientSock = accept(m_serverSocket, nullptr, nullptr);
        if (clientSock == INVALID_SOC)
        {
            report_error("Accept failed");
            continue;
        }
        std::cout << "[SERVER] Client connected\n";
        std::thread(&TCPServer::handleClient, this, clientSock).detach();
    }
}
