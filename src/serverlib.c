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
    free(request->method);
    free(request->path);
    free(request->version);
    free(request->host);
    free(request->user_agent);
    free(request->accept);
    free(request->accept_language);
    free(request->accept_encoding);
    free(request->connection);
    free(request->upgrade_insecure_requests);
    free(request);
}

HTTPResponse *initHTTPResponse()
{
    HTTPResponse *response = malloc(sizeof(HTTPResponse));
    response->content_type = malloc(100 * sizeof(char));
    response->content_length = malloc(15 * sizeof(char));
    response->content = NULL;
    response->binary = 0;
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
        response->header = "HTTP/1.1 404 Not Found\r\n";
        // Set the file path to the 404 page
        strcpy(file_path, "www/html/404.html");
    }
    else {
        response->header = "HTTP/1.1 200 OK\r\n";
    }

    // Complete the contents fields according to the file type
    if (strstr(file_path, ".html") != NULL)
    {
        strcpy(response->content_type, "text/html");
        char *content = readFile(file_path);
        sprintf(response->content_length, "%ld", strlen(content));
        response->content = content;
        response->binary = 0;
    }
    else if (strstr(file_path, ".css") != NULL)
    {
        strcpy(response->content_type, "text/css");
        char *content = readFile(file_path);
        sprintf(response->content_length, "%ld", strlen(content));
        response->content = content;
        response->binary = 0;
    }
    else if (strstr(file_path, ".gif") != NULL)
    {
        size_t size = 0;
        strcpy(response->content_type, "image/gif");
        response->file_data = readImage(file_path, &size);
        response->content = "";
        sprintf(response->content_length, "%ld", size);
        response->binary = 1;
    }
    else if (strstr(file_path, "webp") != NULL)
    {
        size_t size = 0;
        strcpy(response->content_type, "image/webp");
        response->file_data = readImage(file_path, &size);
        response->content = "";
        sprintf(response->content_length, "%ld", size);
        response->binary = 1;
    }
    else if (strstr(file_path, "png") != NULL)
    {
        size_t size = 0;
        strcpy(response->content_type, "image/png");
        response->file_data = readImage(file_path, &size);
        response->content = "";
        sprintf(response->content_length, "%ld", size);
        response->binary = 1;
    }
    else if (strstr(file_path, ".pdf") != NULL) {
        size_t size = 0;
        strcpy(response->content_type, "application/pdf");
        response->file_data = readImage(file_path, &size);
        response->content = "";
        sprintf(response->content_length, "%ld", size);
        response->binary = 1;
    }
    return response;
}

char *unparseHTTPResponse(HTTPResponse *response)
{
    char *type = "Content-Type: ";
    char *length = "Content-Length: ";
    char *end = "\r\n";
    response->response_size = 0;
    if (response->binary == 1)
    {
        // cast content_length to int to avoid warning
        int content_length = atoi(response->content_length);
        response->response_size = (strlen(response->header) + strlen(type) + strlen(response->content_type) + strlen(length) + strlen(response->content_length) + 3 * strlen(end)) * sizeof(char) + content_length + 1;
    }
    else {
        response->response_size = (strlen(response->header) + strlen(type) + strlen(response->content_type) + strlen(length) + strlen(response->content_length) + 3 * strlen(end) + strlen(response->content)) * sizeof(char) + 1;
    }
    char *buffer = malloc(response->response_size);
    strcpy(buffer, response->header);
    strcat(buffer, type);
    strcat(buffer, response->content_type);
    strcat(buffer, end);
    strcat(buffer, length);
    strcat(buffer, response->content_length);
    strcat(buffer, end);
    strcat(buffer, end);


    if (response->binary == 1)
    {
        memcpy(buffer + strlen(buffer), response->file_data, atoi(response->content_length));
    }
    else {
        strcat(buffer, response->content);
    }
    return buffer;
}

    

void printHTTPResponse(HTTPResponse *response)
{
    printf("Server response:\n");
    color("00");
    color("34");
    char * buffer = unparseHTTPResponse(response);
    printf("%s\n\n", buffer);
    color("00");
    color("37");
    color("01");
}

void freeHTTPResponse(HTTPResponse *response)
{
    free(response->header);
    free(response->content_type);
    free(response->content_length);
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
    strcat(content, "\0");
    fclose(file);
    return content;
}

void * readImage(char *file_path, size_t *size) {
    // Read size of file
    FILE *file = fopen(file_path, "rb");

    // Get file size
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read file
    void *content = malloc(*size);
    fread(content, 1, *size, file);

    return content;
}

