# Simple Chat Server and Client

This repository contains a simple chat server and client implemented in C++. The server allows multiple clients to connect simultaneously and exchange messages with each other.

## Features
- Supports multiple clients connecting concurrently.
- Clients can send messages to all other connected clients.
- Basic error handling for client disconnections.

## Dependencies
- C++ compiler (e.g., g++)
- operating system (e.g.,Windows)

## Usage
### Compilation:
Compile the server and client programs separately using a C++ compiler.
```bash
g++ server.cpp -o server
g++ client.cpp -o client
