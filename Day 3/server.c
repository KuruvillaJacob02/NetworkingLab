#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    server_socket = socket(AF_INET, SOCK_STREAM, 0);	//Creates server Socket
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;  //Stores Socket address
    server_addr.sin_addr.s_addr = INADDR_ANY;  //Stores IP address  server_addr.sin_addr.s_addr = inet(" ")
    server_addr.sin_port = htons(PORT);  //Stores Port Number

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept a connection from a client
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
    if (client_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data from client and send response
    while (1){
	    recv(client_socket, buffer, BUFFER_SIZE, 0);
	    printf("From client: %s\n", buffer);
	    bzero(buffer, sizeof(buffer));
	    int n = 0;
	    printf("Enter message for client: ");
	    while ((buffer[n++] = getchar()) != '\n') 
            ; 
   	    send(client_socket, buffer, strlen(buffer), 0);
   	    printf("Waiting for Client..\n");
	     if (strncmp("exit",buffer,4) == 0) {
		    printf("Server Exits");
		    break;
		}
	    
    }
    close(client_socket);
    close(server_socket);

    return 0;
}
