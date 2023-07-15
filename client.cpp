#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr;
    int bytes_sent, bytes_received;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
        error("ERROR opening socket");

    
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT);

    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
        error("ERROR connecting");

    std::cout << "Connected to server. You can start typing your message." << std::endl;

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        std::cout << "> ";
        std::cin.getline(buffer, BUFFER_SIZE);

        
        bytes_sent = send(sockfd, buffer, strlen(buffer), 0);
        if (bytes_sent == SOCKET_ERROR)
            error("ERROR writing to socket");


        bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received == SOCKET_ERROR)
            error("ERROR reading from socket");

        buffer[bytes_received] = '\0';
        std::cout << "Server response: " << buffer << std::endl;
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
