#pragma once
#include <string>
#include <vector>
#include "PacketStructs.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define closesocket close
#endif

class SocketUtils {
public:
    SocketUtils();
    ~SocketUtils();
    
    bool connect(const std::string& host, int port);
    bool sendRequest(const RequestPacket& packet);
    std::vector<ResponsePacket> receivePackets();
    bool requestResend(uint8_t sequence);
    void disconnect();

private:
    SOCKET sock;
    bool initializeWinsock();
    int32_t convertBigEndian(int32_t value);
};