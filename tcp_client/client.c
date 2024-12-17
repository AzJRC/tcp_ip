/* client.c */
#include "client.h"


// ./client {host} -> stdout(Header information)
int main(int argc, char *argv[]) {	
		
	AppArgs app_args;	
	
	// argument parser
	if (parse_arguments(argc, argv, &app_args) == -1) {
		return -1;
	}

	//variables
	int *version_ip = &app_args.version_ip;
	char **host_ip = &app_args.host_ip;

	// tmp until -v 6 is implemented
	if (*version_ip == 6) {
		fprintf(stdout, "TCP v6 client not implemented yet");
		return 0;
	}
	
	char *data;
	

	// Create socket for endpoint commnunication
	int sfd = create_socket(*version_ip);
	if (sfd == -1) {
		return handle_errors("socket", errno);
	}

	// Connect to the socket		
	if (connect_to_host(sfd, *host_ip, *version_ip) == -1) { 
		close(sfd);
		return handle_errors("connect", errno);
	}

	/*
		Man pages socket(2) Once connected,  data  may  be transferred using read(2) and write(2) calls
		or some variant of the send(2) and recv(2) calls.	
		When a session has been completed a close(2) may be performed.
	*/

	data = HTTP_REQ;

	send_request(sfd, data);

	char resp[BUFF_SIZE];
	if (recv_response(sfd, resp, sizeof(resp)) == 0) {
		printf("\n%s\n", resp);
	}

	return 0;
}
