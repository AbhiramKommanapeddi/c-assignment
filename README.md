# c-assignment

# ABX Exchange Client

## Overview

A C++ client application that connects to the ABX exchange server to fetch stock ticker data and store it in a JSON format. The client ensures all packet sequences are received without any missing data.

## Prerequisites

- CMake (3.10 or higher)
- C++ compiler with C++17 support
- Node.js (version 16.17.0 or higher)
- nlohmann/json library (automatically fetched by CMake)

## Project Structure

```
C++ Assignment/
├── CMakeLists.txt      - Build configuration
├── PacketStructs.h     - Packet data structures
├── SocketUtils.h       - Socket utility header
├── SocketUtils.cpp     - Socket utility implementation
└── main.cpp           - Main client application
```

## Building the Project

1. Install CMake:

   - Download from https://cmake.org/download/
   - Install and ensure "Add CMake to system PATH" is selected

2. Build steps:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running the Application

1. Start the ABX exchange server:

```bash
node main.js
```

2. In a separate terminal, run the client:

```bash
./build/abx_client
```

## Features

- TCP socket communication with ABX exchange server
- Automatic handling of big-endian byte order
- Detection and recovery of missing packet sequences
- JSON output generation with complete sequence data
- Cross-platform support (Windows/Linux)
- Robust error handling

## Output Format

The application generates `output.json` with the following structure:

```json
[
  {
    "symbol": "MSFT", // 4-byte ticker symbol
    "type": "B", // Buy/Sell indicator (B/S)
    "quantity": 100, // Order quantity
    "price": 15000, // Order price
    "sequence": 1 // Packet sequence number
  }
  // ... more packets ...
]
```

## Error Handling

- Connection failures to server
- Missing packet sequences
- Network communication errors
- Invalid data formats
- Socket initialization errors

## Technical Details

- Server Port: 3000
- Protocol: TCP
- Byte Order: Big-endian for all numeric fields
- Packet Types:
  - Stream All Packets (Type 1)
  - Resend Packet (Type 2)

## Notes

- The server automatically closes connection after sending all packets
- Last packet sequence is guaranteed to be received
- All missing sequences are automatically requested and recovered
- Data integrity is verified before saving to JSON

## Troubleshooting

- Ensure Node.js server is running before starting the client
- Check firewall settings if connection fails
- Verify CMake and C++ compiler are properly installed
- Ensure port 3000 is available for the server

## License

This project is part of a technical assessment and should be kept private unless otherwise specified.
