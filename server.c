// Author : Mathias Citrini
// Created: 2023/03/29
// Updated: 2023/03/29
// Version: 1.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>

#define PORT 8080
#define BACKLOG 10

// Part A : Web server simply dumps HTTP request messages to the console.

int main(int argc, char const *argv[])
{
    // Socket file descriptor

    // Creating socket

    // Server parameters

    // Bind socket to port

    // Listen for connections

    // Infinite loop
    while (1)
    {
        // Select connection

        // Accept connection
    }

    // Close all connections
    return 0;
}

// gcc -o server server.c