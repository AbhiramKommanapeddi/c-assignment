#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <nlohmann/json.hpp>
#include "SocketUtils.h"

using json = nlohmann::json;

class ABXClient {
public:
    bool connect(const std::string& host, int port) {
        return socket.connect(host, port);
    }

    std::vector<ResponsePacket> getAllPackets() {
        // Request initial stream
        RequestPacket request{1, 0};
        if (!socket.sendRequest(request)) {
            std::cerr << "Failed to send initial request" << std::endl;
            return {};
        }

        // Receive all packets
        auto packets = socket.receivePackets();
        
        // Find missing sequences
        if (!packets.empty()) {
            int maxSeq = packets.back().sequence;
            std::set<int> receivedSeqs;
            for (const auto& packet : packets) {
                receivedSeqs.insert(packet.sequence);
            }

            // Request missing sequences
            for (int seq = 1; seq <= maxSeq; ++seq) {
                if (receivedSeqs.find(seq) == receivedSeqs.end()) {
                    if (socket.requestResend(seq)) {
                        auto resendPacket = socket.receivePackets();
                        if (!resendPacket.empty()) {
                            packets.push_back(resendPacket[0]);
                        }
                    }
                }
            }
        }

        return packets;
    }

    void saveToJson(const std::vector<ResponsePacket>& packets, const std::string& filename) {
        json output = json::array();
        
        for (const auto& packet : packets) {
            json packetJson;
            packetJson["symbol"] = std::string(packet.symbol, 4);
            packetJson["type"] = std::string(1, packet.buySellIndicator);
            packetJson["quantity"] = packet.quantity;
            packetJson["price"] = packet.price;
            packetJson["sequence"] = packet.sequence;
            
            output.push_back(packetJson);
        }

        std::ofstream file(filename);
        file << output.dump(2);
    }

private:
    SocketUtils socket;
};

int main() {
    ABXClient client;
    
    if (!client.connect("127.0.0.1", 3000)) {
        std::cerr << "Failed to connect to server" << std::endl;
        return 1;
    }

    auto packets = client.getAllPackets();
    if (packets.empty()) {
        std::cerr << "No packets received" << std::endl;
        return 1;
    }

    client.saveToJson(packets, "output.json");
    std::cout << "Successfully received and saved " << packets.size() << " packets to output.json" << std::endl;

    return 0;
}