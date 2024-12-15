/* client.c */
#include "client.h"


#define PORT 80


// ./client {host} -> stdout(Header information)
int main(int argc, char *argv[]) {
	
	if (argc < 2) {
		fprintf(stderr, "Usage: ./client {host}");
		return -1;
	};
	const char *host_ip = argv[1];

	const int v4_domain = AF_INET;
	const int tcp_sock = SOCK_STREAM;
	const int proto = 0;
	
	char *data;
	char buff[512];
	
	/*
		Man pages socket(2) say A connection to another socket is created with a connect(2) call.  
		struct sockaddr_in {
			sa_family_t     sin_family;     AF_INET
			in_port_t       sin_port;       Port number
			struct in_addr  sin_addr;       IPv4 address
		};

		struct in_addr {
			in_addr_t s_addr;
		};
	*/

	struct sockaddr_in sock;
	sock.sin_family = AF_INET;
	sock.sin_port = htons(PORT);
	sock.sin_addr.s_addr = inet_addr(host_ip);

	// Create socket for endpoint commnunication
	int s = socket(v4_domain, tcp_sock, proto);
	if (s == -1) {
		return printerr("socket", errno);
	}

	// Connect to the socket	
	if (connect(s, (struct sockaddr *)&sock, sizeof(sock)) == -1) {
		close(s);
		return printerr("connect", errno);
	}

	/*
		Man pages socket(2) Once connected,  data  may  be transferred using read(2) and write(2) calls
		or some variant of the send(2) and recv(2) calls.	
		When a session has been completed a close(2) may be performed.
	*/

	data = "HEAD / HTTP/1.0\r\n\r\n";

	if (write(s, data, strlen(data)) == -1) {
		close(s);
		return printerr("write", errno);
	}
	
	memset(buff, 0, sizeof(buff));  // make sure buff is clean
	if (read(s, buff, sizeof(buff)-1) == -1) {
		close(s);
		return printerr("read", errno);
	}
	
	printf("\n%s\n", buff);	
	return 0;
}