#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<error.h>

#define PORT 8081
#define BUFFER_SIZE 1024

void chat(int connectionID){
    char buffer[BUFFER_SIZE];
    
    memset(buffer,0,BUFFER_SIZE);
    recv(connectionID,buffer,BUFFER_SIZE,0);
    
    if (strncmp(buffer,"HELO",4) == 0){
        printf("Connection Established..\n");
        send(connectionID, buffer, BUFFER_SIZE, 0);
    }
    
    int n = 0;
    while(1){
        memset(buffer,0,BUFFER_SIZE);
        recv(connectionID,buffer,BUFFER_SIZE,0);
        if (strncmp(buffer,"QUIT",4) == 0){
            printf("Server Exit..");
            break;
        }
        
        if (n == 0){
            printf("From: %s",buffer);
            n++;
        }
            
        else if (n == 1){
            printf("To: %s",buffer);
            n++;
        }
            
        else if (n == 2){
            printf("Data: %s",buffer);
            n = 0;
        }
        
    }
}

void main(){
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address, client_address;
    int sockID = socket(AF_INET, SOCK_STREAM,0);
    
    if (sockID == -1){
        perror("Socket creation failed");
        exit(1);
    }
    
    else
        printf("Socket created successfully..\n");
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    
    if (bind(sockID,(struct sockaddr*)&server_address,sizeof(server_address)) != 0){
        perror("Socket bind failed");
        exit(1);
    }
    else
        printf("Socket successfully binded..\n");
    
    if (listen(sockID,5) != 0){
        perror("Server listening failed");
        exit(1);
    }
    
    else
        printf("Sever listening..\n");
    
    int address_size = sizeof(client_address);
    int connectionID = accept(sockID,(struct sockaddr*)&client_address,&address_size);
    
    if (connectionID < 0){
        perror("Server accept failed");
        exit(1);
    }
    
    else
        printf("Server accepts client..\n");
    
    chat(connectionID);
}

