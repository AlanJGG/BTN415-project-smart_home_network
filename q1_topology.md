# Smart Home Network Automation System

## 1. Network

Lighting Subnet:
- Subnet Address (192.168.1.0/26)
- Device Type (Lighting systems)
- Example: Light1 (192.168.1.10)
- Purpose: Controls lighting operations, like turning lights on/off and checking status.

Thermostat Subnet
- Subnet Address (192.168.1.64/26)
- Device Type (Temperature control systems)
- Example: Thermo1 (192.168.1.70)
- Purpose: Manages temperature settings and monitoring.

Camera Subnet
- Subnet Address (192.168.1.192/26)
- Device Type (Security Cameras)
- Example: Camera1 (192.168.1.200)
- Purpose: Handles functions such as surveillance, recording, and monitoring.

---

## 2. Communications

- Client: Sends HTTP-like commands.
- Server: Processes requests and manages device states.
- Devices: Act as endpoints simulated through the network.

---

## 3. Network Interaction 
- Intra-subnet communication: Devices communicate within the same subnet using ARP.
- Intra-subnet communication: Packets are routed through a router using static routing.
- ARP resolution: Used to map IP addresses to MAC addresses before communication.

This typology is for improving scalability and organization by separating the device categories into different subnets, reducing broadcast domains and improving routing efficiency.
