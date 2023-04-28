#include "../include/serverlib.h"

void dumpError(char *message)
{
    color("31");
    printf("\n");
    perror(message);
}

void printError(char * message)
{
    color("31");
    printf("%s\n", message);
    color("37");
}

void checkArguments(int argc, char const *argv[])
{
    if (argc != 2)
    {
        color("31");
        printError("Usage: ./server <port>");
        exit(1);
    }
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
        exit(1);
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
        exit(1);
    }

    // Listen
    printf("Listening...\n");
    if (listen(sockfd, backlog) == -1)
    {
        dumpError("listen");
        exit(1);
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

/***
 * Get the next HTTP field (e.g. Host, User-Agent, etc.)
 * @param token token to get the next field
 * @return the next HTTP field
*/
static char *getHTTPField(char **token)
{
    // Get the next token
    *token = strtok(NULL, " ");
    *token = strtok(NULL, "\n");
    return *token;
}

HTTPRequest *parseHTTPRequest(char buffer[1024])
{
    if (strlen(buffer) == 0)
    {
        return NULL;
    }
    // Create a copy of the buffer
    char copy[1024];
    strcpy(copy, buffer);
    // Initialize the HTTP request
    HTTPRequest *request = initHTTPRequest();
    const char *delim = " \n";
    // Get method
    char *token = strtok(copy, delim);
    strcpy(request->method, token);
    // Get path
    token = strtok(NULL, delim);
    strcpy(request->path, token);
    // Get Version
    token = strtok(NULL, delim);
    strcpy(request->version, token);
    // Get Fields
    request->host = getHTTPField(&token);
    request->user_agent = getHTTPField(&token);
    request->accept = getHTTPField(&token);
    request->accept_language = getHTTPField(&token);
    request->accept_encoding = getHTTPField(&token);
    request->connection = getHTTPField(&token);
    request->upgrade_insecure_requests = getHTTPField(&token);
    // Return the parsed HTTP request
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

HTTPResponse *initHTTPResponse()
{
    HTTPResponse *response = malloc(sizeof(HTTPResponse));
    response->content_type = malloc(100 * sizeof(char));
    response->content_length = malloc(15 * sizeof(char));
    response->content = NULL;
    response->startline = NULL;
    response->file_data = NULL;
    response->binary = 0; // 0: text, 1: binary
    return response;
}

HTTPResponse *createHTTPResponse(HTTPRequest *request)
{
    // Create response
    HTTPResponse *response = initHTTPResponse();

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
        response->startline = "HTTP/1.1 404 Not Found\r\n";
        // Set the file path to the 404 page
        strcpy(file_path, "www/html/404.html");
    }
    else
    {
        response->startline = "HTTP/1.1 200 OK\r\n";
    }

    // Get the extension of the accessed file
    char *ext = strrchr(file_path, '.');
    ext++;
    printf("File path: %s\n", file_path);
    printf("Extension of the accessed file: %s\n", ext);

    // Set the content type and length and the content of the response based on the extension
    if (strcmp(ext, "html") == 0 || strcmp(ext, "css") == 0 || strcmp(ext, "js") == 0 || strcmp(ext, "txt") == 0)
    {
        // Text
        strcpy(response->content_type, "text/");
        strcat(response->content_type, ext);
        // Read the file
        char *content = readFile(file_path);
        sprintf(response->content_length, "%ld", strlen(content));
        response->content = content;
        // Set binary to 0 to indicate that the content is not binary
        response->binary = 0;
    }
    else
    {
        size_t size = 0;
        // Images
        if (strcmp(ext, "jpg") == 0 || strcmp(ext, "png") == 0 || strcmp(ext, "gif") == 0 || strcmp(ext, "webp") == 0 || strcmp(ext, "svg") == 0 || strcmp(ext, "ico") == 0 || strcmp(ext, "jpeg") == 0)
            strcpy(response->content_type, "image/");
        // Audio and video
        else if (strcmp(ext, "mp3") == 0)
            strcpy(response->content_type, "audio/");
        else if (strcmp(ext, "mp4") == 0)
            strcpy(response->content_type, "video/");
        // Application
        else if (strcmp(ext, "json") == 0 || strcmp(ext, "pdf") == 0)
            strcpy(response->content_type, "application/");
        // Default = Binary
        else
            strcpy(response->content_type, "application/octet-stream");
        // Add the extension to the content type
        if (strcmp(response->content_type, "application/octet-stream") != 0)
            strcat(response->content_type, ext);
        // Read the file
        response->file_data = readFileBinary(file_path, &size);
        response->content = "";
        // Set the content length
        sprintf(response->content_length, "%ld", size);
        // Set binary to 1 to indicate that the content is binary
        response->binary = 1;
    }
    return response;
}

char *unparseHTTPResponse(HTTPResponse *response)
{
    // Initialize the header
    char *type = "Content-Type: ";
    char *length = "Content-Length: ";
    char *end = "\r\n";
    response->response_size = 0;
    if (response->binary == 1)
    {
        // Cast content_length to int to avoid warning
        int content_length = atoi(response->content_length);
        // Calculate the size of the response
        response->response_size = (strlen(response->startline) + strlen(type) + strlen(response->content_type) + strlen(length) + strlen(response->content_length) + 3 * strlen(end)) * sizeof(char) + content_length + 1;
    }
    else
    {
        // Calculate the size of the response
        response->response_size = (strlen(response->startline) + strlen(type) + strlen(response->content_type) + strlen(length) + strlen(response->content_length) + 3 * strlen(end) + strlen(response->content)) * sizeof(char) + 1;
    }
    // Allocate memory for the response
    char *buffer = malloc(response->response_size);
    // Create the response
    strcpy(buffer, response->startline);
    strcat(buffer, type);
    strcat(buffer, response->content_type);
    strcat(buffer, end);
    strcat(buffer, length);
    strcat(buffer, response->content_length);
    strcat(buffer, end);
    strcat(buffer, end);
    // Add the content to the response
    if (response->binary == 1)
    {
        // Add the binary file data to the response
        memcpy(buffer + strlen(buffer), response->file_data, atoi(response->content_length));
    }
    else
    {
        // Add the text content to the response
        strcat(buffer, response->content);
    }
    return buffer;
}

void printHTTPResponse(HTTPResponse *response)
{
    printf("Server response:\n");
    color("00");
    color("34");
    char *buffer = unparseHTTPResponse(response);
    printf("%s\n\n", buffer);
    color("00");
    color("37");
    color("01");
}

/***
 * Get the size of a file
 * @param file The file
 * @return The size of the file
*/
static size_t getFileSize(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return size;
}

char *readFile(char *file_path)
{
    // Open the Text file (HTML, CSS, JS, TXT)
    FILE *file = fopen(file_path, "r");
    // Allocate memory for the content
    size_t size = getFileSize(file);
    char *content = malloc(size * sizeof(char));
    // Read the file
    fread(content, 1, size, file);
    strcat(content, "\0");
    // Close the file
    fclose(file);
    return content;
}

void *readFileBinary(char *file_path, size_t *size)
{
    // Read size of file
    FILE *file = fopen(file_path, "rb");
    // Get the size of the file
    *size = getFileSize(file);
    // Read file
    void *content = malloc(*size);
    fread(content, 1, *size, file);
    // Close the file
    fclose(file);
    return content;
}