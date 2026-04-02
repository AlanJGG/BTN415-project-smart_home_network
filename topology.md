# Smart Home Network Automation System

## 1. Network Design & Subnetting

The Smart Home Network is divided into multiple subnets using equal-size subnetting (/26) for isolating device categories, reducing broadcast traffic, and improving scalability.

Details:
| Subnet     | Network Address  | Range                         | Broadcast     | Devices                   |
| ---------- | ---------------- | ----------------------------- | ------------- | ------------------------- |
| Lighting   | 192.168.1.0/26   | 192.168.1.1 – 192.168.1.62    | 192.168.1.63  | Light1 (192.168.1.10)     |
| Thermostat | 192.168.1.64/26  | 192.168.1.65 – 192.168.1.126  | 192.168.1.127 | Thermo1 (192.168.1.70)    |
| Security   | 192.168.1.128/26 | 192.168.1.129 – 192.168.1.190 | 192.168.1.191 | (Optional future devices) |
| Camera     | 192.168.1.192/26 | 192.168.1.193 – 192.168.1.254 | 192.168.1.255 | Camera1 (192.168.1.200)   |

Why /26?
Each subnet contains 64 total addresses with 62 usable hosts. Block size 64 will create subnet boundaries at multiples of 64 (0, 64,128,192). This simplifies the routing because the router can now determine the destination subnet by checking which 64-address block contains the target IP. The /26 mask was chosen over other options because it provides adequate address space for each device category while maintaining simple, predictable boundaries.

---

## 2. Routing Design

A router is used for enabling communication between different subnets throughout static routing.

| Destination Network | Gateway            |
| ------------------- | ------------------ |
| 192.168.1.0/26      | Lighting Gateway   |
| 192.168.1.64/26     | Thermostat Gateway |
| 192.168.1.128/26    | Security Gateway   |
| 192.168.1.192/26    | Camera Gateway     |

Routing Behavior: 
-Same subnet (intra-subnet) -> Direct communication using ARP
-Different subnet (inter-subnet) -> Packet sent to router, router forwards based on routing table

---

## 3. ARP Integration
Addressed Resolution Protocol (ARP) is used for map IP addressed to MAC addressed before packet tranmission.

-Process:
1. Device checks ARP table
2. If MAC not found -> ARP request
3. MAC address resolved
4. Packet is sent

This ensures realistic LAN communication, required before any packet delivery.

---

## 4. Communication Architecture

- Client: Sends HTTP-like commands (e.g. GET /light/on).
- Server: Handles requests using TCP sockets, used multithreading for concurrency, dispatches commands to devices, dispatches commands to devices.
- Devices: Represent Iot components (light, thermostat, camera), maintain internal state, execute commands.

---

## 5. Network Interaction 
- Intra-subnet communication: Devices communicate within the same subnet using ARP.
- Intra-subnet communication: Packets are routed through a router using static routing.
- ARP resolution: Used to map IP addresses to MAC addresses before communication.

---

## 6. Network Interaction Flow Example

1. Client sends: GET /camera/on
2. Server:
   - Parses requests
   - Calls sendPacket()
3. Network Simulation
   - ARP resolves MAC address
   - Router determines correct subnet
4. Packet delivere to device
5. Device executes:
   - Camera turns ON
6. Response sent back to client

This typology is for improving scalability and organization by separating the device categories into different subnets, reducing broadcast domains and improving routing efficiency.
