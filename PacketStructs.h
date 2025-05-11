#pragma once
#include <cstdint>
#include <string>

// Request packet structure
struct RequestPacket {
    uint8_t callType;
    uint8_t resendSeq;
};

// Response packet structure
struct ResponsePacket {
    char symbol[4];        // 4 bytes for symbol (ASCII)
    char buySellIndicator; // 1 byte for buy/sell indicator (ASCII)
    int32_t quantity;      // 4 bytes for quantity
    int32_t price;        // 4 bytes for price
    int32_t sequence;     // 4 bytes for sequence number
};