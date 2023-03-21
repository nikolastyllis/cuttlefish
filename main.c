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

int main(int argc, char* argv[]) {
	
	char buffer[BUFF_SIZE];
    	struct sockaddr_in address;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	int port = (int) *argv[2];
	char* host = argv[1];
	char* hello = "Hello\n";

	address.sin_family = DOMAIN;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
    	address.sin_port = htons(port);	
	
	int proxyfd = socket(DOMAIN, TYPE, PROTOCAL);
	int bind_attempt = bind(proxyfd, (struct sockaddr*)&address, addrlen);
	if(bind_attempt) perror("Binding to port failed");	
	
	int listen_attempt = listen(proxyfd, 10);
	if(listen_attempt) perror("Failed to listen");
	
	while(1) {
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
