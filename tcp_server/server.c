/* server.c */
#include "server.h"


// ./server {port}
int main(int argc, char *argv[]) {	
	
	AppArgs app_args;
	if (parse_arguments(argc, argv, &app_args) == -1) {
		return -1;
	}
	
	//variable
	int *version_ip = &app_args.version_ip;
	int *lport = &app_args.lport;
	
	// define a socket
	int sfd;
	if ((sfd = create_socket(*version_ip)) == -1) {
		return handle_errors("socket", errno);
	}

	// bind socket to a local address (IPv4)
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(*lport);
	srv_addr.sin_addr.s_addr = 0;	
	
	if (bind(sfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1) {
		close(sfd);
		return handle_errors("bind", errno);
	}	

	// listen for incoming connections
	if (listen(sfd, MAX_CONN) == -1) {
		close(sfd);
		return handle_errors("listen", errno);
	};	
	fprintf(stdout, "Listening for a maximum of %i connections...\n", MAX_CONN);

	// accept connections
	int acc_sfd;
	socklen_t size_srv;
	if ((acc_sfd = accept(sfd, (struct sockaddr *)&srv_addr, &size_srv)) == -1) {
		close(sfd);
		return handle_errors("accept", errno);
	}
	
	// process new connections
	char buff[512];	
	printf("New connnection detected!\n");
	read(acc_sfd, buff, sizeof(buff)-1);
	char *data = "httpd v1.0\n";
	write(acc_sfd, data, strlen(data));

	printf("%s", buff);

	close(sfd);
	return 0;
}
