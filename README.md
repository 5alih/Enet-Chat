# Basic ENet Chat Application

A simple client-server networking application built using ENet. This project serves as a starting point for building networked applications, demonstrating basic UDP communication with reliability.

## Features

- Basic server that accepts a client connection
- Simple client that can connect to the server
- Message exchange between client and server
- Graceful server shutdown with 'quit' command

## Prerequisites

To build and run this application, you need:

- C++ compiler with C++11 support
- ENet library
- pthread library (for Linux/Unix systems)

### Installing ENet

#### Ubuntu/Debian
```bash
sudo apt-get install libenet-dev
```

#### macOS (using Homebrew)
```bash
brew install enet
```

#### Windows
Download and build from source or use vcpkg:
```bash
vcpkg install enet:x64-windows
```

## Building the Application

### Linux/macOS
```bash
# Compile the server
g++ server.cpp -o server -lenet -pthread

# Compile the client
g++ client.cpp -o client -lenet
```

### Windows (using MSVC)
```bash
# Compile the server
cl server.cpp /I<path-to-enet-include> /link <path-to-enet-lib>

# Compile the client
cl client.cpp /I<path-to-enet-include> /link <path-to-enet-lib>
```

## Running the Application

1. Start the server:
```bash
./server
```
The server will start listening on port 1453. Type 'quit' to gracefully shut down the server.

2. In another terminal, start the client:
```bash
./client
```
The client will attempt to connect to localhost:1453 and send a test message.

## Project Structure

- `server.cpp`: Server implementation that listens for connections and echoes messages back to clients
- `client.cpp`: Client implementation that connects to the server and sends a test message

## Understanding the Code

### Server Features
- Listens on port 1453
- Handles client connectiona/disconnectiona
- Sends back success message back to clienta
- Supports graceful shutdown via 'quit' command
- Uses 1 channel for now

### Client Features
- Connects to server on localhost:1453
- Sends initial test message
- Receives and displays server responses
- Handles disconnection events

## Network Protocol

The current implementation uses a simple protocol:
- Channel 0: Reserved for chat messages (reliable delivery)

## License

This project is open source and available under the MIT License.

## Acknowledgments

This project uses the ENet reliable UDP networking library:
- ENet: http://enet.bespin.org/
