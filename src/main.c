#include "../include/serverlib.h"

#define BACKLOG 10

int main(int argc, char const *argv[])
{
    /* -------------- Part A -------------- */
    // Prepare console
    system("clear");
    color("01");
    color("37");

    // Check arguments
    if (argc != 2)
    {
        printError("Usage: ./server <port>");
        exit(1);
    }

    printf("Hello sir, welcome to my server !\n");
    printf("You chose to run the server on port: %s\n", argv[1]);
    printf("Default IP address is: 127.0.0.1\n\n");

    color("35");
    printf("- The part A of the project is to dump the HTTP request to the console.\n");
    printf("- The part B of the project is to send a HTTP response to the client.\n");
    printf("Both parts are implemented in this program.\n\n");
    color("37");

    printf("A test website is available at http://<IP address>:<port>/index.html\n");
    printf("If you want to test partA, please type 1\n");
    printf("If you want to test both parts, please type 2\n");
    printf("1 or 2 ? ");
    int choice;
    scanf("%d", &choice);
    system("clear");

    // Port number
    int port = atoi(argv[1]);
    // socket file descriptor
    int sockfd = createSocket();
    int new_fd;
    // server address
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    // size of client address
    unsigned int sin_size;

    setServerAddress(&server_addr, port);
    startServer(&server_addr, BACKLOG, sockfd);

    while (true)
    {
        // Accept connection
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1)
        {
            dumpError("accept");
            continue;
        }
        printf("Server got connection from %s !\n", inet_ntoa(client_addr.sin_addr));

        // Receive data
        char buffer[1024];
        int numbytes;
        if ((numbytes = recv(new_fd, buffer, 1024, 0)) == -1)
        {
            dumpError("recv");
            exit(1);
        }
        buffer[numbytes] = '\0';
        // Parse HTTP request
        HTTPRequest * received = parseHTTPRequest(buffer);
        if (received == NULL)
        {
            printError("Buffer is not a valid HTTP request ! Skipping...\n");
            continue;
        }
        // Part A prerequisites - Dump HTTP request to consoles
        printHTTPRequest(received);
        // Continue or exit
        if (choice == 1)
        {
            continue;
        }
        /* -------------- Part B -------------- */
        // Part B prerequisites - Create HTTP response
        HTTPResponse * response = createHTTPResponse(received);
        printHTTPResponse(response);
        // Part B prerequisites - Send HTTP response
        char *responseStr = unparseHTTPResponse(response);
        size_t responseSize = response->response_size;
        if (send(new_fd, responseStr, responseSize, 0) == -1)
        {
            perror("send");
            exit(1);
        }
        // Free memory
        free(response);
        freeHTTPRequest(received);

        // Close connection
        close(new_fd);
    }
    close(sockfd);
    return 0;
}