#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#define PORT 8089
#define BUFFER_SIZE 1024

void main(){
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address;
    int sockID = socket(AF_INET, SOCK_STREAM,0); //Protocol 0 for TCP
    
    if (sockID == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    
    else
      printf("Socket successfully created..\n");
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    
    if (connect(sockID, (struct sockaddr*)&server_address, sizeof(server_address)) != 0){
        perror("Connection with server failed");
        exit(1);
    }
    
    else
        printf("Connected to the server..\n");
    
    while (1){
        int n = 0;
        bzero(buffer,sizeof(buffer));
        printf("Enter message for Server: ");
        while ((buffer[n++] = getchar()) != '\n');
        send(sockID,buffer,sizeof(buffer),0);
        bzero(buffer,sizeof(buffer));
        recv(sockID, buffer,sizeof(buffer),0);
        printf("Message from Server: %s",buffer);
        if (strncmp(buffer,"exit",4) == 0){
            printf("Client Exit..");
            break;
        }
    }
    close(sockID);
}
