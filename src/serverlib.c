#include "../include/serverlib.h"

static void dumpError(char *message)
{
    color("31");
    printf("\n");
    perror(message);
    exit(1);
}

void setServerAddress(struct sockaddr_in *server_addr, int port)
{
    // Set server address
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    server_addr->sin_addr.s_addr = INADDR_ANY;
    printf("Server's address: 127.0.0.1:%d\n", port);
}

int createSocket()
{
    // Create socket
    int sockfd;
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        dumpError("socket");
    }
    color("32");
    printf("Socket created successfully\n");
    color("37");
    return sockfd;
}

void startServer(struct sockaddr_in *server_addr, int backlog, int sockfd)
{
    // Bind socket
    color("30");
    printf("Binding socket...\n");
    if (bind(sockfd, (struct sockaddr *)server_addr, sizeof(struct sockaddr)) == -1)
    {
        dumpError("bind");
    }

    // Listen
    printf("Listening...\n");
    if (listen(sockfd, backlog) == -1)
    {
        dumpError("listen");
    }

    color("32");
    printf("Server has been started successfully\n\n");
    color("37");
}

HTTPRequest *initHTTPRequest()
{
    HTTPRequest *request = malloc(sizeof(HTTPRequest));
    request->method = malloc(10 * sizeof(char));
    request->path = malloc(100 * sizeof(char));
    request->version = malloc(10 * sizeof(char));
    request->host = malloc(100 * sizeof(char));
    request->user_agent = malloc(100 * sizeof(char));
    request->accept = malloc(100 * sizeof(char));
    request->accept_language = malloc(100 * sizeof(char));
    request->accept_encoding = malloc(100 * sizeof(char));
    request->connection = malloc(100 * sizeof(char));
    request->upgrade_insecure_requests = malloc(10 * sizeof(char));
    return request;
}

static char *getHTTPField(char **token)
{
    *token = strtok(NULL, " ");
    *token = strtok(NULL, "\n");
    return *token;
}

HTTPRequest *parseHTTPRequest(char buffer[1024])
{
    char copy[1024];
    strcpy(copy, buffer);
    if (strlen(copy) == 0)
    {
        printf("Buffer is empty\n");
        return initHTTPRequest();
    }
    HTTPRequest *request = initHTTPRequest();
    const char *delim = " \n";
    // Get method
    char *token = strtok(copy, delim);
    strcpy(request->method, token);
    // Get path
    token = strtok(NULL, delim);
    strcpy(request->path, token);
    // Get version
    token = strtok(NULL, delim);
    strcpy(request->version, token);
    // Get host
    request->host = getHTTPField(&token);
    // Get user agent
    request->user_agent = getHTTPField(&token);
    // Get accept
    request->accept = getHTTPField(&token);
    // Get accept language
    request->accept_language = getHTTPField(&token);
    // Get accept encoding
    request->accept_encoding = getHTTPField(&token);
    // Get connection
    request->connection = getHTTPField(&token);
    // Get upgrade insecure requests
    request->upgrade_insecure_requests = getHTTPField(&token);
    return request;
}

void printHTTPRequest(HTTPRequest *request)
{
    color("00");
    color("33");
    printf("Method: %s\n", request->method);
    printf("Path: %s\n", request->path);
    printf("Version: %s\n", request->version);
    printf("Host: %s\n", request->host);
    printf("User agent: %s\n", request->user_agent);
    printf("Accept: %s\n", request->accept);
    printf("Accept language: %s\n", request->accept_language);
    printf("Accept encoding: %s\n", request->accept_encoding);
    printf("Connection: %s\n", request->connection);
    printf("Upgrade insecure requests: %s\n\n", request->upgrade_insecure_requests);
    color("37");
    color("01");
}

void freeHTTPRequest(HTTPRequest *request)
{
    
    
}

HTTPResponse *initHTTPResponse()
{
    HTTPResponse *response = malloc(sizeof(HTTPResponse));
    response->version = malloc(10 * sizeof(char));
    response->status_code = malloc(10 * sizeof(char));
    response->status_message = malloc(100 * sizeof(char));
    response->date = malloc(100 * sizeof(char));
    response->server = malloc(100 * sizeof(char));
    response->content_type = malloc(100 * sizeof(char));
    response->content_length = malloc(15 * sizeof(char));
    response->content = NULL;
    response->connection = malloc(100 * sizeof(char));
    return response;
}

HTTPResponse *createHTTPResponse(HTTPRequest *request)
{
    // Create response
    HTTPResponse *response = initHTTPResponse();
    strcpy(response->version, "HTTP/1.1");
    strcpy(response->date, "Thu, 01 Jan 1970 00:00:00 GMT");
    strcpy(response->server, "ComputerNetworkProject");
    strcpy(response->status_code, "200");
    strcpy(response->status_message, "OK");
    strcpy(response->connection, "keep-alive");

    // Get the file path
    char *path = request->path;
    if (strcmp(path, "/") == 0)
    {
        path = "/index.html";
    }
    char *file_path = malloc(100 * sizeof(char));
    strcpy(file_path, "www");
    strcat(file_path, path);

    // Check if the file exists
    if (access(file_path, F_OK) != 0)
    {
        // File not found
        strcpy(response->status_code, "404");
        strcpy(response->status_message, "Not Found");
        strcpy(response->connection, "close");
        // Set the file path to the 404 page
        strcpy(file_path, "www/404.html");
    }

    // Check if the file is an html file
    if (strstr(file_path, ".html") != NULL)
    {
        strcpy(response->content_type, "text/html");
        char *content = readFile(file_path);
        sprintf(response->content_length, "%ld", strlen(content));
        response->content = content;
    }
    else if (strstr(file_path, ".css") != NULL)
    {
        strcpy(response->content_type, "text/css");
        char *content = readFile(file_path);
        sprintf(response->content_length, "%ld", strlen(content));
        response->content = content;
    }
    else if (strstr(file_path, ".gif") != NULL)
    {
        strcpy(response->content_type, "image/gif");
        response->content = readImage(file_path);
        sprintf(response->content_length, "%ld", strlen(response->content));
        printf("Image Set\n");
    }
    else if (strstr(file_path, ".pdf") != NULL) {
        strcpy(response->content_type, "application/pdf");
        response->content = readImage(file_path);
        sprintf(response->content_length, "%ld", strlen(response->content));
        printf("PDF Set\n");
    }
    return response;
}

char *unparseHTTPResponse(HTTPResponse *response)
{
    size_t size = strlen(response->version) + strlen(response->status_code) + strlen(response->status_message) + strlen(response->content_type) + strlen(response->content_length) + strlen(response->connection) + strlen(response->date) + strlen(response->server) + 100;
    char *buffer = malloc(size * sizeof(char));
    strcpy(buffer, response->version);
    strcat(buffer, " ");
    strcat(buffer, response->status_code);
    strcat(buffer, " ");
    strcat(buffer, response->status_message);
    strcat(buffer, "\n");
    strcat(buffer, "Connection: ");
    strcat(buffer, response->connection);
    strcat(buffer, "\n");
    strcat(buffer, "Date: ");
    strcat(buffer, response->date);
    strcat(buffer, "\n");
    strcat(buffer, "Server: ");
    strcat(buffer, response->server);
    strcat(buffer, "\n\n");
    if (response->content != NULL)
        buffer = realloc(buffer, (size + strlen(response->content)) * sizeof(char));
        strcat(buffer, response->content);
    return buffer;
}

void printHTTPResponse(HTTPResponse *response)
{
    color("00");
    color("33");
    printf("Version: %s\n", response->version);
    printf("Status code: %s\n", response->status_code);
    printf("Status message: %s\n", response->status_message);
    printf("Date: %s\n", response->date);
    printf("Server: %s\n", response->server);
    printf("Content type: %s\n", response->content_type);
    printf("Content length: %s\n", response->content_length);
    printf("Connection: %s\n", response->connection);
    printf("Content: %s\n\n", response->content);
    color("37");
    color("01");
}

void freeHTTPResponse(HTTPResponse *response)
{
    free(response->version);
    free(response->status_code);
    free(response->status_message);
    free(response->date);
    free(response->server);
    free(response->content_type);
    free(response->content_length);
    free(response->connection);
    free(response->content);
    free(response);
}

char *readFile(char *file_path)
{
    // Open the HTML file
    FILE *file = fopen(file_path, "r");
    // Copy the file content to the response content
    char *content = malloc(10000 * sizeof(char));
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            strcat(content, "");
        }
        else
        {
            char *s = malloc(2 * sizeof(char));
            s[0] = c;
            s[1] = '\0';
            strcat(content, s);
        }
    }
    printf("Reading finished\n");
    strcat(content, "\0");
    fclose(file);
    return content;
}

char * readImage(char *file_path) {
    // Read size of file
    FILE *file = fopen(file_path, "rb");
    size_t size = 0;

    // Get file size
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read file
    char *content = malloc(size * sizeof(char));
    size_t byte_reads = fread(content, 1, size, file);

    // Encode content to base64
    const char *b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t b64_size = (size + 2) / 3 * 4 +1;
    char *b64_content = (char*)malloc(b64_size * sizeof(char));

    // Encode 3 bytes at a time
    size_t i = 0;
    size_t j = 0;
    for (i = 0; i < size; i += 3) {
        // Read 3 bytes
        unsigned char bytes[3];
        bytes[0] = (i + 0 < size) ? content[i + 0] : 0;
        bytes[1] = (i + 1 < size) ? content[i + 1] : 0;
        bytes[2] = (i + 2 < size) ? content[i + 2] : 0;

        // Encode 3 bytes to 4 characters
        b64_content[j++] = b64[bytes[0] >> 2];
        b64_content[j++] = b64[((bytes[0] & 0x3) << 4) | (bytes[1] >> 4)];
        b64_content[j++] = b64[((bytes[1] & 0xF) << 2) | (bytes[2] >> 6)];
        b64_content[j++] = b64[bytes[2] & 0x3F];
    }

    // Add padding
    for (i = 0; i < (3 - size % 3) % 3; i++) {
        b64_content[b64_size - 1 - i] = '=';
    }

    // Transform to string
    b64_content[b64_size] = '\0';
    

    fclose(file);

    return b64_content;
}

