/*
    * ServerLib
    * A simple HTTP server library
    * Author: Mathias CITRINI
    * License: MIT
    * Version: 1.0.0
    * Github: https://github.com/DevilMortar/ComputerNetworkProject
*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <stdbool.h>
#include <math.h>

/***
 * Color the text in the terminal
 * @param x color code
*/
#define color(x) printf("\033[%sm", x)

/***
 * HTTPRequest structure
 * @param method HTTP method
 * @param path HTTP path
 * @param version HTTP version
 * @param host HTTP host
 * @param user_agent HTTP user agent
 * @param accept HTTP accept
 * @param accept_language HTTP accept language
 * @param accept_encoding HTTP accept encoding
 * @param connection HTTP connection
 * @param upgrade_insecure_requests HTTP upgrade insecure requests
*/
typedef struct HTTPRequest
{
    char *method;
    char *path;
    char *version;
    char *host;
    char *user_agent;
    char *accept;
    char *accept_language;
    char *accept_encoding;
    char *connection;
    char *upgrade_insecure_requests;
} HTTPRequest;

/***
 * HTTPResponse structure to store the HTTP response data. You can unparse this structure with the "unparseHTTPResponse" function 
 * to get the HTTP response in string format.
 * @param startline HTTP startline
 * @param date HTTP date
 * @param content_type HTTP content type
 * @param content_length HTTP content length
 * @param content HTTP content
 * @param file_data HTTP file data
 * @param response_size HTTP response size
 * @param binary How to read the file (0: text, 1: binary)
*/
typedef struct HTTPResponse
{
    char *startline;
    char *content_type;
    char *content_length;
    char *content;
    void *file_data;
    size_t response_size;
    bool binary;
} HTTPResponse;

/***
 * @brief Exit the program if the command line arguments are not valid
 * @param argc number of arguments
 * @param argv arguments
*/
void checkArguments(int argc, char const *argv[]);

/***
 * Dump the error message in the console (color: red)
 * @param message error message
*/
void dumpError(char *message);

/***
 * Print the error message in the console (color: red)
 * @param message error message
*/
void printError(char *message);

/***
 * Set the sockaddr_in structure with the server address
 * @param server_addr server address to set
 * @param port port number
 * @return void
 */
void setServerAddress(struct sockaddr_in *server_addr, int port);

/***
 * Start the server
 * @param server_addr server address
 * @param backlog number of connections allowed on the incoming queue
 * @param sockfd socket file descriptor
 * @return void
*/
void startServer(struct sockaddr_in *server_addr, int backlog, int sockfd);

/***
 * Create a socket
 * @return socket file descriptor
 */
int createSocket();

/***
 * Parse the HTTP request
 * @param buffer buffer containing the HTTP request
 * @return Pointer to HTTPRequest structure
 */
HTTPRequest * parseHTTPRequest(char buffer[1024]);

/***
 * Initialize the HTTPRequest structure
 * @return Pointer to HTTPRequest structure
 */
HTTPRequest * initHTTPRequest();

/***
 * Print the HTTPRequest structure
 * @param request Pointer to HTTPRequest structure
*/
void printHTTPRequest(HTTPRequest * request);

/***
 * Initialize the HTTPResponse structure
 * @return Pointer to HTTPResponse structure
*/
HTTPResponse * initHTTPResponse();

/***
 * Create an HTTP response
 * @param request Pointer to HTTPRequest structure
 * @return Pointer to HTTPResponse structure
*/
HTTPResponse * createHTTPResponse(HTTPRequest * request);

/***
 * Unparse the HTTP response
 * @param response Pointer to HTTPResponse structure
 * @return Pointer to char array
*/
char * unparseHTTPResponse(HTTPResponse * response);

/***
 * Print the HTTPResponse structure
 * @param response Pointer to HTTPResponse structure
*/
void printHTTPResponse(HTTPResponse * response);

/***
 * Read HTML file
 * @param path Path to the HTML file
 * @return Pointer to char array
*/
char * readFile(char * file_path);

/***
 * Read Image file
 * @param path Path to the Image file
 * @return Pointer to char array
*/
void *readFileBinary(char *file_path, size_t *size);