/* server.h */
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
#define DEF_LISTEN_PORT 8081
#define DEF_VER_IP 4
#define MAX_CONN 3
#define BUFF_SIZE 512


// function prototypes
int parse_arguments(int argc, char *argv[], int *version_ip, int *lport);
int create_socket(int version_ip);
int _create_v4_socket();
int _create_v6_socket();
int handle_errors(const char *err_fuct, int errno_code);


int parse_arguments(int argc, char *argv[], int *version_ip, int *lport) {
		// Clean memory spaces of arguments
		memset(version_ip, 0, sizeof(*version_ip));
		memset(lport, 0, sizeof(*lport));

		// prrocess arguments with parser
		int opt;
		while ((opt = getopt(argc, argv, "v::p::")) != -1) {
			switch(opt) {
				case 'v':
					if (optarg == NULL) break;
					*version_ip = atoi(optarg);
					break;
				case 'p':
					if (optarg == NULL) break;
					if (atoi(optarg) <= 1023) {
						fprintf(stderr, "Listening port must be a number higher than 1023.");
						return -1;
					}
					*lport = atoi(optarg);
					break;
				default:
					fprintf(stderr, "Usage: %s [-v ip_version] [-p port]\n", argv[0]);
					return -1;
			}
		}
		
		if (*version_ip == 0) {
			*version_ip = DEF_VER_IP;
		}
		if (*lport == 0) {
			*lport = DEF_LISTEN_PORT;
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


int handle_errors(const char *err_funct, int  errno_code) {
		/*
			Print the str name of an errno code.
			Return: -1
		*/
		fprintf(stderr, "Error with %s(): %s", err_funct, strerror(errno_code));
		return -1;
}
