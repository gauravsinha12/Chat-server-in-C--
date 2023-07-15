#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    WSADATA wsaData;
    SOCKET sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    int num_clients = 0;
    char buffer[BUFFER_SIZE];
    std::vector<SOCKET> client_sockets;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
        error("ERROR opening socket");

    // Initialize server address struct
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
        error("ERROR on binding");

    // Listen for incoming connections
    if (listen(sockfd, MAX_CLIENTS) == SOCKET_ERROR)
        error("ERROR on listening");

    std::cout << "Server started. Listening on port " << PORT << std::endl;

    while (true) {
        // Accept incoming connection
        int clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd == INVALID_SOCKET)
            error("ERROR on accept");

        client_sockets.push_back(newsockfd);
        num_clients++;

        // Handle communication with the client
        while (true) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_received = recv(newsockfd, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received == SOCKET_ERROR)
                error("ERROR reading from socket");
            if (bytes_received == 0) {
                std::cout << "Client disconnected" << std::endl;
                break;
            }

            std::cout << "Received message from client: " << buffer << std::endl;

            // Broadcast message to all other clients
            for (int i = 0; i < client_sockets.size(); i++) {
                if (client_sockets[i] != newsockfd) {
                    int bytes_sent = send(client_sockets[i], buffer, strlen(buffer), 0);
                    if (bytes_sent == SOCKET_ERROR)
                        error("ERROR writing to socket");
                }
            }
        }

        // Close the connection with the client
        closesocket(newsockfd);
    }

    // Cleanup
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
