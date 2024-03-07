#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
 
    while (1){
    	    int n =0;
	    printf("Enter message for server: ");
	    while ((buffer[n++] = getchar()) != '\n')
	    	;
	    send(client_socket, buffer, strlen(buffer), 0);
	    printf("Waiting for Server..\n");
	    bzero(buffer,sizeof(buffer));
	    recv(client_socket, buffer, BUFFER_SIZE, 0);
	    printf("From Server : %s", buffer);
	    if (strncmp("exit", buffer,4) == 0) {
	    	    printf("Client Exit");
		    break;
	    }
    }
    close(client_socket);

    return 0;
}
