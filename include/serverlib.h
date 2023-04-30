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

/* --- Define the structure for the project --- */

/***
 * HTTPField structure to store the HTTP fields
 * @param name field name
 * @param value field value
 * @param next pointer to the next field
*/
typedef struct HTTPField
{
    char *name;
    char *value;
    struct HTTPField *next;
} HTTPField;

/***
 * HTTPRequest structure
 * @param method HTTP method
 * @param path HTTP path
 * @param version HTTP version
 * @param firstField HTTP fields
 * @param lastField HTTP fields
*/
typedef struct HTTPRequest
{
    char *method;
    char *path;
    char *version;
    HTTPField *firstField;
    HTTPField *lastField;
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

/* ------------------------------------ PART A ------------------------------------ */

/* --- Server status --- */

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

/* --- Server Launch --- */

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

/* --- HTTP Request Handling --- */

/***
 * Parse the HTTP request dynamically to get all the fields
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
 * Free the HTTPRequest structure
 * @param request Pointer to HTTPRequest structure
*/
void freeHTTPRequest(HTTPRequest * request);

/* ------------------------------------ PART B ------------------------------------ */

/* --- HTTP Response Handling --- */

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

/* --- File Handling --- */

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