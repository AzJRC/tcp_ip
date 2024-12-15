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


// Methods
int printerr(char *err_funct, int errno_code) {
		/*
			Print the str name of an errno code.
			Return: -1
		*/
		fprintf(stderr, "Error with %s(): %s", err_funct, strerror(errno_code));
		return -1;
}


