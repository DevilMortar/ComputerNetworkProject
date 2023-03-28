// Author : Mathias Citrini
// Created: 2023/03/29
// Updated: 2023/03/29
// Version: 1.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <socket.h>

#define PORT 8080
#define BACKLOG 10

// Part A : Web server simply dumps HTTP request messages to the console.

int main(int argc, char const *argv[])
{
    int socket_fd, new_fd;
    struct sockaddr_in server;
    struct sockaddr_in client;

    int sin_size;

    if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(socket_fd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    // Accepting connections

    while (true)
    {
        sin_size = sizeof(struct sockaddr_in);
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }
        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
    }
    // Close all connections
    close(socket_fd);
    close(new_fd);
    return 0;
}
