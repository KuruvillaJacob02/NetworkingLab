#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#define PORT 8089
#define BUFFER_SIZE 1024

void main(){
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address, client_address;
    int sockID = socket(AF_INET, SOCK_STREAM,0); //Protocol 0 for TCP
    int connectionID;
    if (sockID == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    
    else
      printf("Socket successfully created\n");
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; 
    server_address.sin_port = htons(PORT);
    
    if (bind(sockID, (struct sockaddr*)&server_address, sizeof(server_address)) != 0){
        perror("Socket bind failed\n");
        exit(1);
    }
    else
      printf("Socket Successfully binded..\n");
   
   if (listen(sockID,5) != 0){
      perror("Listening failed");
      exit(1);
   }
   else
        printf("Server Listening..\n");
    
    int address_size = sizeof(client_address);
    connectionID = accept(sockID, (struct sockaddr*)&client_address, &address_size);
    
    if (connectionID < 0){
        perror("Server Accept failed..");
        exit(1);
    }
    
    else
        printf("Server accepts Client..\n");
    
    while(1){
        int n =0;
        bzero(buffer,sizeof(buffer));
        recv(connectionID,buffer,sizeof(buffer),0);
        printf("From Client: %s\n",buffer);
        bzero(buffer,sizeof(buffer));
        printf("Enter message for Client: ");
        while ((buffer[n++] = getchar()) != '\n')
          ;
        send(connectionID,buffer,sizeof(buffer),0);
        if (strncmp(buffer,"exit",4) == 0){
            printf("Server Exit..");
            break;
        }
        
    }
    close(connectionID);
    close(sockID);
}
