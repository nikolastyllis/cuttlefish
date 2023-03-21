#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define DOMAIN AF_INET
#define TYPE SOCK_STREAM
#define PROTOCAL 0
#define SERVER_LOOP while(1)

void initAddress(struct sockaddr_in* address, char* port) {
	address->sin_family = DOMAIN;
	address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(atoi(port));	
}

int main(int argc, char** argv) {

	//Initialise address
    struct sockaddr_in address;
	initAddress(&address, argv[1]);

	//Get size of an address
	socklen_t addrlen = sizeof(struct sockaddr_in);

	//Initialise buffers
	char buffer[BUFF_SIZE];
	char* hello = "Server says hello!\n";

	//Create socket
	int proxyfd = socket(DOMAIN, TYPE, PROTOCAL);

	//Bind to port
	int bind_attempt = bind(proxyfd, (struct sockaddr*)&address, addrlen);
	if(bind_attempt) perror("Binding to port failed");	
	
	//Begin listening
	int listen_attempt = listen(proxyfd, 0);
	if(listen_attempt) perror("Failed to listen");
	
	//Main server loop
	SERVER_LOOP {
		int client = accept(proxyfd, (struct sockaddr*)&address, &addrlen);
		if(client<0) perror("Failes to accept connection");
		
		send(client, hello, strlen(hello), 0);
		
		read(client, buffer, BUFF_SIZE);
		printf("Recieved message: %s\n", buffer);

		close(client);
	}

	shutdown(proxyfd, SHUT_RDWR);
	return 0;
}	
