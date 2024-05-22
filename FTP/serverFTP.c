#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<fcntl.h>

#define BUFFER_SIZE 1024
#define PORT 8089


void chat(int connectionID){
    char buffer[BUFFER_SIZE], filename[BUFFER_SIZE], toBuffer[BUFFER_SIZE], dataBuffer[BUFFER_SIZE];
    
    while(1){
    
        memset(filename,0,BUFFER_SIZE);
        memset(buffer,0,BUFFER_SIZE);
        recv(connectionID,filename,BUFFER_SIZE,0);
        if (strncmp(filename, "QUIT", 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
        
        printf("From Client : %s\n",filename);
        
        FILE *fptr = fopen(filename, "r");
        
        if (fptr == NULL){
            perror("File not found");
            send(connectionID,"-1",strlen("-1"),0);
            continue;
        }
        fgets(buffer,BUFFER_SIZE,fptr);
        fclose(fptr);
        send(connectionID,buffer,sizeof(buffer),0);
    }
}      
        

void main(){
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
    
    close(sockID);
    close(connectionID);
}
