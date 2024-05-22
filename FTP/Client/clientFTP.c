#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<fcntl.h>

#define BUFFER_SIZE 1024
#define PORT 8089

void chat(int sockID){
    char buffer[BUFFER_SIZE], filename[BUFFER_SIZE], toBuffer[BUFFER_SIZE], dataBuffer[BUFFER_SIZE];
  
    while(1){
        memset(buffer, 0, BUFFER_SIZE);
        memset(filename, 0, BUFFER_SIZE);
        
        printf("Enter file to be searched (QUIT to exit): ");
        scanf("%s",filename);
        
        if (strncmp(filename,"QUIT",4)==0){
            printf("Client exits..\n");
            send(sockID,filename,sizeof(filename),0);
            break;
        }
        
        send(sockID,filename,sizeof(filename),0);
        recv(sockID,buffer,sizeof(buffer),0);
        
        if (strncmp(buffer,"-1",2) == 0){
            continue;
        }
        FILE *fptr = fopen(filename, "w");
        fprintf(fptr, "%s", buffer);
        fclose(fptr);
        printf("\nFile Created: %s\n", filename);
        printf("Content: %s\n",buffer);
    }
    
}      
        

void main(){
    struct sockaddr_in server_address;
    int sockID = socket(AF_INET, SOCK_STREAM,0);
    
    if (sockID == -1){
        perror("Socket creation failed");
        exit(1);
    }
    
    else
        printf("Socket created successfully..\n");
      
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    
    if (connect(sockID,(struct sockaddr*)&server_address,sizeof(server_address)) != 0){
        perror("Cannot connect to server");
        exit(1);
    }
    else
        printf("Successfully connected to server..\n");
        
    chat(sockID);
    
    close(sockID);
}
