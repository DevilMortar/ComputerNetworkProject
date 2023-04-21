#include "../include/serverlib.h"

#define BACKLOG 10

int main(int argc, char const *argv[])
{
    system("clear");
    color("01");
    color("37");
    // Check arguments
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    // Port number
    int port = atoi(argv[1]);

    // socket file descriptor
    int sockfd = createSocket();
    int new_fd;
    // server address
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    // size of client address
    int sin_size;

    setServerAddress(&server_addr, port);
    startServer(&server_addr, BACKLOG, sockfd);

    while (true)
    {
        // Accept connection
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }
        printf("Server got connection from %s !\n", inet_ntoa(client_addr.sin_addr));

        // Receive data
        char buffer[1024];
        int numbytes;
        if ((numbytes = recv(new_fd, buffer, 1024, 0)) == -1)
        {
            perror("recv");
            exit(1);
        }

        buffer[numbytes] = '\0';
        HTTPRequest * received = parseHTTPRequest(buffer);
        if (received == NULL)
        {
            printf("Buffer is not a valid HTTP request ! Skipping...\n");
            continue;
        }
        // Part A prerequisites - Dump HTTP request to consoles
        printHTTPRequest(received);
        HTTPResponse * response = createHTTPResponse(received);
        // Send HTTP response to client
        char *responseStr = unparseHTTPResponse(response);
        printHTTPResponse(response);
        if (send(new_fd, responseStr, response->response_size, 0) == -1)
        {
            perror("send");
        }
        free(response);
        free(received);

        // Close connection
        close(new_fd);
    }
    close(sockfd);
    return 0;
}