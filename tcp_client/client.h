/* client.h */
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>


// Definitions
#define PORT 80
#define DEF_VER_IP 4
#define BUFF_SIZE 512
#define HTTP_REQ "HEAD / HTTP/1.0\r\n\r\n"


// function prototypes
int parse_arguments(int argc, char *argv[], int *version_ip, char *host_ip[]);
int create_socket(int version_ip);
int _create_v4_socket();
int _create_v6_socket();
int connect_to_host(int sfd, const char *host_ip, int version_ip);
int _connect_to_v4host(int sfd, const char *host_ip);
int _connect_to_v6host(int sfd, const char *host_ip);
int handle_errors(const char *err_fuct, int errno_code);
int send_request(int sfd, const char *req);
int recv_response(int sfd, char *buff, size_t buff_size);


int parse_arguments(int argc, char *argv[], int *version_ip, char *host_ip[]) {
		int opt;
		if (argc == 1) {
				fprintf(stderr, "Usage: %s [-v ip_version] -h ip\n", argv[0]);
				return -1;	
		}
		while ((opt = getopt(argc, argv, "v::h:")) != -1) {
			switch(opt) {
				case 'v':
					if (optarg == NULL) break;
					*version_ip = atoi(optarg);
					break;
				case 'h':
					*host_ip = optarg;
					break;
				default:
					fprintf(stderr, "Usage: %s [-v ip_version] -h {ip}\n", argv[0]);
					return -1;
			}
		}
		if (*version_ip != 4 && *version_ip != 6) {
			fprintf(stderr, "Invalid IP version: %i. Supported values are 4 and 6.\n", *version_ip);
			return -1;
		}
		if (*host_ip == NULL) {
			fprintf(stderr, "Missing Host IP after -h.\n");
			return -1;
		}

		return 0;
}


int create_socket(int version_ip) {
	switch(version_ip) {
		case 4:
			return _create_v4_socket();
		case 6:
			return _create_v6_socket();
		default:
			fprintf(stderr, "Invalid IP version: %i. Supported values are 4 and 6.\n", version_ip);
			return -1;
	}
}


int _create_v4_socket() {
	return socket(AF_INET, SOCK_STREAM, 0);
}


int _create_v6_socket() {
	fprintf(stdout, "TCP v6 client not implemented yet");
	return -1;
}


int connect_to_host(int sfd, const char *host_ip, int version_ip) {
	switch(version_ip) {
		case 4:
			return _connect_to_v4host(sfd, host_ip);
		case 6:
			return _connect_to_v6host(sfd, host_ip);
		default:
			fprintf(stderr, "Invalid IP version: %i. Supported values are 4 and 6.\n", version_ip);
			return -1;
	}	
}


int _connect_to_v4host(int sfd, const char *host_ip) {
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
	
	return connect(sfd, (struct sockaddr *)&sock, sizeof(sock));
}


int _connect_to_v6host(int sfd, const char *host_ip) {
	fprintf(stdout, "TCP v6 client not implemented yet");
	return -1;
}


int handle_errors(const char *err_funct, int  errno_code) {
		/*
			Print the str name of an errno code.
			Return: -1
		*/
		fprintf(stderr, "Error with %s(): %s", err_funct, strerror(errno_code));
		return -1;
}


int send_request(int sfd, const char *req) {
	if (write(sfd, req, strlen(req)) == -1) {
		close(sfd);
		return handle_errors("write", errno);
	}
	return 0;
}


int recv_response(int sfd, char *buff, size_t buff_size) {
	ssize_t bytes_read = read(sfd, buff, buff_size - 1);
	if (bytes_read == -1) {
		close(sfd);
		return handle_errors("read", errno);
	}
	buff[bytes_read] = '\0';
	return 0;
}
