#include "SocketUtils.h"
#include <iostream>
#include <cstring>

SocketUtils::SocketUtils() : sock(INVALID_SOCKET) {
    initializeWinsock();
}

SocketUtils::~SocketUtils() {
    disconnect();
#ifdef _WIN32
    WSACleanup();
#endif
}

bool SocketUtils::initializeWinsock() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true;
#endif
}

bool SocketUtils::connect(const std::string& host, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);

    return ::connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != INVALID_SOCKET;
}

int32_t SocketUtils::convertBigEndian(int32_t value) {
    return htonl(value);
}

bool SocketUtils::sendRequest(const RequestPacket& packet) {
    return send(sock, (const char*)&packet, sizeof(packet), 0) == sizeof(packet);
}

std::vector<ResponsePacket> SocketUtils::receivePackets() {
    std::vector<ResponsePacket> packets;
    ResponsePacket packet;
    
    while (true) {
        int bytesRead = recv(sock, (char*)&packet, sizeof(packet), 0);
        if (bytesRead <= 0) break;
        
        // Convert from big-endian
        packet.quantity = ntohl(packet.quantity);
        packet.price = ntohl(packet.price);
        packet.sequence = ntohl(packet.sequence);
        
        packets.push_back(packet);
    }
    
    return packets;
}

bool SocketUtils::requestResend(uint8_t sequence) {
    RequestPacket packet{2, sequence};
    return sendRequest(packet);
}

void SocketUtils::disconnect() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}