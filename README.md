# Smart Home Network Automation System
### BTN 415 – Data Communications Programming

---

## 1. Project Overview

This system allows users to control smart home devices (lights, thermostat, security camera) over a TCP/IP network using a custom HTTP-like protocol written in C++.

**Key features:**
- Multithreaded TCP server — handles many clients simultaneously
- One dedicated worker thread per device (light, thermostat, camera)
- Simulated network layer: ARP resolution + static routing between subnets
- Cross-platform: compiles on Linux, macOS, and Windows

---

## 2. Directory Structure

```
btn415-smart_home_network/
      Makefile
      README.md
      src/
            network/                    <- Network simulation layer
                  Device.h / .cpp       <- Smart home endpoint (name, IP, MAC)
                  ARPTable.h / .cpp     <- IP to MAC resolution (simulates ARP)
                  Subnet.h / .cpp       <- Groups devices under a /26 subnet
                  Router.h / .cpp       <- Static routing table
                  NetworkUtils.h / .cpp <- sendPacket() - ARP + routing trace

            utilities/                  <- Cross-platform socket helpers
                  SocketUtils.h         <- socket_t, INVALID_SOC, close_socket()
                  SocketSystem.h / .cpp <- RAII Winsock init (Windows only)

            protocol/                   <- Application layer
                  ProtocolParser.h      <- Request / Response structs
                  ProtocolParser.cpp    <- Parses "GET /device/action[/value]"

            server/
                  TCPServer.h / .cpp    <- Main server (sockets + threads + devices)
                  main_server.cpp       <- Entry point for server binary

            client/
                  TCPClient.h / .cpp    <- Client (connect, send, receive)
                  main_client.cpp       <- Entry point for client binary
```

---

## 3. Architecture Overview

When a client sends a command (e.g. GET /light/on), the server's accept() loop
receives the connection and spawns a new thread running handleClient(). That thread
reads the raw text from the socket and passes it to ProtocolParser, which splits it
into device, action, and value fields. dispatchRequest() then calls sendPacket() to
simulate ARP resolution and routing, and enqueues a DeviceCommand into the matching
device queue (light, thermostat, or camera). Each device has a permanent worker thread
sleeping on a condition_variable — it wakes when a command arrives, executes the
action, and sends the full network trace + result back to the client over the same
TCP socket.

---

## 4. Network Design

The network uses **VLSM /26 subnetting** of the 192.168.1.0/24 address space. Each device type lives in its own subnet.

### Subnet Table

| Subnet          | Network Address  | Mask               | Usable Range                      | Device    | IP             | MAC             |
|-----------------|------------------|--------------------|-----------------------------------|-----------|----------------|-----------------|
| Lighting        | 192.168.1.0      | 255.255.255.192 /26| 192.168.1.1 – 192.168.1.62       | Light1    | 192.168.1.10   | AA:BB:CC:DD:01  |
| Thermostat      | 192.168.1.64     | 255.255.255.192 /26| 192.168.1.65 – 192.168.1.126     | Thermo1   | 192.168.1.70   | AA:BB:CC:DD:02  |
| Security        | 192.168.1.128    | 255.255.255.192 /26| 192.168.1.129 – 192.168.1.190    | Security1 | 192.168.1.130  | AA:BB:CC:DD:03  |
| Cameras         | 192.168.1.192    | 255.255.255.192 /26| 192.168.1.193 – 192.168.1.254    | Camera1   | 192.168.1.200  | AA:BB:CC:DD:04  |

> A /26 subnet gives 64 addresses (62 usable hosts) per segment.

### Static Routing Table

| Destination     | Gateway             |
|-----------------|---------------------|
| 192.168.1.0/26  | Lighting Gateway    |
| 192.168.1.64/26 | Thermostat Gateway  |
| 192.168.1.192/26| Camera Gateway      |

### ARP Table (pre-populated)

| IP Address     | MAC Address    |
|----------------|----------------|
| 192.168.1.10   | AA:BB:CC:DD:01 |
| 192.168.1.70   | AA:BB:CC:DD:02 |
| 192.168.1.200  | AA:BB:CC:DD:04 |

---

## 5. How to Build

### Requirements

| Platform       | Requirement                          |
|----------------|--------------------------------------|
| Linux / macOS  | `g++` with C++17, `make`             |
| Windows        | MinGW-w64 or MSVC with C++17         |

### Build both binaries

```bash
make
```

This produces two executables in the project root:
- `server` — the TCP server
- `client` — the interactive client

### Build individually

```bash
make server   # build only the server
make client   # build only the client
make clean    # remove compiled binaries
```

---

## 6. How to Run

### Step 1 — Start the server

Open a terminal and run:

```bash
./server
```

Expected output:
```
[NET] Network topology initialised:
  Lighting subnet   : 192.168.1.0/26   (Light1  @ 192.168.1.10)
  Thermostat subnet : 192.168.1.64/26  (Thermo1 @ 192.168.1.70)
  Camera subnet     : 192.168.1.192/26 (Camera1 @ 192.168.1.200)
[THREAD] Light worker started       (tid 1234)
[THREAD] Thermostat worker started  (tid 1235)
[THREAD] Camera worker started      (tid 1236)
[SERVER] Listening on port 8080 ...
```

### Step 2 — Start the client

Open a **second terminal** and run:

```bash
./client
```

To connect to a different host or port:

```bash
./client <server_ip> <port>
# Example:
./client 192.168.1.10 8080
```

### Step 3 — Multiple clients (concurrent test)

Open two or three terminals and run `./client` in each. The server handles all of them simultaneously.

---

## 7. Command Reference

All commands follow the format:

```
GET /<device>/<action>[/<value>]
```

### Light

| Command               | Description            |
|-----------------------|------------------------|
| `GET /light/on`       | Turn the light ON      |
| `GET /light/off`      | Turn the light OFF     |
| `GET /light/status`   | Query the light state  |

### Thermostat

| Command                      | Description                        |
|------------------------------|------------------------------------|
| `GET /thermostat/set/<temp>` | Set temperature (e.g. `.../set/22`)|
| `GET /thermostat/status`     | Query current temperature          |

### Camera

| Command               | Description                              |
|-----------------------|------------------------------------------|
| `GET /camera/on`      | Power camera ON                          |
| `GET /camera/off`     | Power camera OFF (stops recording too)   |
| `GET /camera/start`   | Start recording (camera must be ON first)|
| `GET /camera/stop`    | Stop recording                           |
| `GET /camera/status`  | Query camera state                       |

### Quit

| Command | Description             |
|---------|-------------------------|
| `q`     | Disconnect and exit     |

---

## 8. Sample Session

```
> GET /light/on

--- Server response ---
[NET] Light1 (192.168.1.10) -> 192.168.1.10
[ARP] Resolved 192.168.1.10 -> AA:BB:CC:DD:01
[ROUTE] Forwarding via Lighting Gateway
200 OK - Light is ON
-----------------------

> GET /thermostat/set/25

--- Server response ---
[NET] Thermo1 (192.168.1.70) -> 192.168.1.70
[ARP] Resolved 192.168.1.70 -> AA:BB:CC:DD:02
[ROUTE] Forwarding via Thermostat Gateway
200 OK - Temperature set to 25 C
-----------------------

> GET /camera/on

--- Server response ---
[NET] Camera1 (192.168.1.200) -> 192.168.1.200
[ARP] Resolved 192.168.1.200 -> AA:BB:CC:DD:04
[ROUTE] Forwarding via Camera Gateway
200 OK - Camera is ON
-----------------------

> GET /camera/start

--- Server response ---
[NET] Camera1 (192.168.1.200) -> 192.168.1.200
[ARP] Resolved 192.168.1.200 -> AA:BB:CC:DD:04
[ROUTE] Forwarding via Camera Gateway
200 OK - Recording started
-----------------------

> GET /unknown/test

--- Server response ---
404 Not Found - Unknown device: unknown
-----------------------

> q
[CLIENT] Disconnecting.
```

---

## 9. Module Descriptions

### `src/network/Device`
Represents a smart home endpoint. Stores name, IP address, and MAC address. Used by ARPTable and sendPacket() to identify endpoints on the simulated network.

### `src/network/ARPTable`
Simulates the Address Resolution Protocol. Maintains an `std::map<string, string>` of IP→MAC entries. The `resolve()` method returns the MAC address if found, or `"ARP_MISS"` to signal a broadcast miss (unknown host).

### `src/network/Subnet`
Groups one or more `Device` objects under a network address and subnet mask. Represents one /26 segment of the topology.

### `src/network/Router`
Static routing table. `addRoute(subnet_base, gateway_label)` populates the table. `routePacket(destIP)` extracts the last octet of the destination IP and finds the matching /26 range (64-address block) to return the gateway label, or `"NO_ROUTE"` if no match.

### `src/network/NetworkUtils`
The `sendPacket()` function ties ARP and routing together. It builds a human-readable trace string, prints it to the server console, and **returns it** so it can be included in the response sent back to the client.

### `src/utilities/SocketUtils`
Header-only. Defines `socket_t`, `INVALID_SOC`, `close_socket()`, and `report_error()` using `#ifdef _WIN32` conditional compilation. Makes the rest of the code platform-agnostic.

### `src/utilities/SocketSystem`
RAII wrapper. Constructor calls `WSAStartup()` on Windows; destructor calls `WSACleanup()`. On Linux/macOS it does nothing. Include it once in `main()`.

### `src/protocol/ProtocolParser`
Tokenises raw socket data into a `Request` struct (`method`, `device`, `action`, `value`, `valid`, `errorMessage`). Rejects non-GET methods and malformed paths before they reach the device layer.

### `src/server/TCPServer`
The core of the project. Responsibilities:
- Creates and configures the listening TCP socket
- Spawns a detached thread per accepted client (`handleClient`)
- On startup, creates three persistent worker threads (one per device)
- Routes parsed requests to the correct device queue via `dispatchRequest()`
- Each device has its own `std::mutex` + `std::condition_variable` + `std::queue<DeviceCommand>`

### `src/client/TCPClient`
Connects to the server, displays the command menu, and runs a read/send/receive loop until the user types `q`.

---

## 10. Threading Model

```
- main()
      - TCPServer::start()
            - thermoWorker <- permanent thread, sleeps on condition_variable
            - cameraWorker <- permanent thread, sleeps on condition_variable
            
      - [accept loop]
            - handleClient(clientA) <- detached thread
            - handleClient(clientB) <- detached thread
            - handleClient(clientC) <- detached thread
```

**Why one thread per device?**

- Commands to different devices execute in parallel (e.g. a camera command doesn't block a thermostat command).
- Commands to the same device are serialised through its queue — no race conditions on shared device state.
- Each device struct also has its own internal `std::mutex` for additional safety.

**Synchronisation primitives used:**

| Primitive              | Where used                                               |
|------------------------|----------------------------------------------------------|
| `std::mutex`           | Per-device command queue, per-device state struct        |
| `std::condition_variable` | Worker threads wait until queue is non-empty          |
| `std::lock_guard`      | Short critical sections (queue push, state access)       |
| `std::unique_lock`     | Worker thread wait loop (required by condition_variable) |
| `std::thread::detach`  | Client handler threads (fire-and-forget per connection)  |

---

## 11. Protocol Specification

### Request

```
GET /<device>/<action>[/<value>] \n
```

- Method: always `GET`
- Device: `light` | `thermostat` | `camera`
- Action: depends on device (see Command Reference)
- Value: optional integer (used by `thermostat/set`)

### Response

```
[NET]   <name> (<src_ip>) -> <dst_ip>
[ARP]   Resolved <ip> -> <mac>   |  ARP Request broadcast...
[ROUTE] Forwarding via <gateway> |  No route found
<status_code> - <result_message>
```

### Status Codes

| Code            | Meaning                                      |
|-----------------|----------------------------------------------|
| `200 OK`        | Command executed successfully                |
| `400 Bad Request` | Malformed request, unknown method          |
| `404 Not Found` | Unknown device or unsupported action         |
