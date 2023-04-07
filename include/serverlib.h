#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> /* for struct sockaddr_in */
#include <arpa/inet.h>  /* for htons and inet_aton */

typedef struct HTTPRequest
{
    char *method;
} HTTPRequest;

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
 * @return HTTPRequest structure
 */
HTTPRequest parseRequest(char *buffer);

/***
 * Initialize the HTTPRequest structure
 * @return HTTPRequest structure
 */
HTTPRequest initHTTPRequest();

