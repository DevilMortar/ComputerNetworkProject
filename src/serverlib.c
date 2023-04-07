#include "../include/serverlib.h"

void setServerAddress(struct sockaddr_in *server_addr, int port)
{
    // Set server address
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    server_addr->sin_addr.s_addr = INADDR_ANY;
    printf("Server's address: %s\n", inet_ntoa(server_addr->sin_addr));
}

int createSocket()
{
    // Create socket
    int sockfd;
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    printf("Socket created successfully\n");
    return sockfd;
}

void startServer(struct sockaddr_in *server_addr, int backlog, int sockfd)
{
    // Bind socket
    if (bind(sockfd, (struct sockaddr *)server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    // Listen
    if (listen(sockfd, backlog) == -1)
    {
        perror("listen");
        exit(1);
    }

    printf("Server has been started successfully\n");
    printf("Server is listening on port %d \n", ntohs(server_addr->sin_port));
    printf("Waiting for connections...\n");
}

HTTPRequest initHTTPRequest()
{
    HTTPRequest request;
    request.method = malloc(10 * sizeof(char));
    return request;
}

HTTPRequest parseRequest(char *buffer)
{
    HTTPRequest request = initHTTPRequest();
}


