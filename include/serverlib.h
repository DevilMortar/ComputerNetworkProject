#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> /* for struct sockaddr_in */
#include <arpa/inet.h>  /* for htons and inet_aton */
#include <stdio.h>
#include <stdlib.h> /* pour exit */
#include <unistd.h> /* pour close */
#include <string.h>     /* pour memset */
#include <unistd.h>     /* pour sleep */
#include <poll.h>
#include <stdbool.h>

#define color(x) printf("\033[%sm", x)

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

typedef struct HTTPResponse
{
    char *version;
    char *status_code;
    char *status_message;
    char *date;
    char *server;
    char *last_modified;
    char *content_type;
    char *content_length;
    char *connection;
    char *content;
} HTTPResponse;

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
 * Free the HTTPRequest structure
 * @param request Pointer to HTTPRequest structure
*/
void freeHTTPRequest(HTTPRequest * request);

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
 * Free the HTTPResponse structure
 * @param response Pointer to HTTPResponse structure
*/
void freeHTTPResponse(HTTPResponse * response);

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
char *readImage(char *file_path);

