CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

SRC_NET  = src/network/Device.cpp src/network/ARPTable.cpp \
           src/network/Subnet.cpp  src/network/Router.cpp  \
           src/network/NetworkUtils.cpp
SRC_UTIL = src/utilities/SocketSystem.cpp
SRC_PROT = src/protocol/ProtocolParser.cpp
SRC_SRV  = src/server/TCPServer.cpp  src/server/main_server.cpp
SRC_CLI  = src/client/TCPClient.cpp  src/client/main_client.cpp

all: server client

server: $(SRC_NET) $(SRC_UTIL) $(SRC_PROT) $(SRC_SRV)
	$(CXX) $(CXXFLAGS) $^ -o server

client: $(SRC_NET) $(SRC_UTIL) $(SRC_PROT) $(SRC_CLI)
	$(CXX) $(CXXFLAGS) $^ -o client

clean:
	rm -f server client
