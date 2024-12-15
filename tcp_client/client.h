/* tcp.h */
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


int printerr(char *err_funct, int errno_code) {
		fprintf(stderr, "Error with %s(): %s", err_funct, strerror(errno_code));
		return -1;
}
