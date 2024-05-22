#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<error.h>

#define PORT 8081
#define BUFFER_SIZE 1024

void chat(int sockID){
    char buffer[BUFFER_SIZE], fromBuffer[BUFFER_SIZE], toBuffer[BUFFER_SIZE], dataBuffer[BUFFER_SIZE];
    int n = 0;
    printf("Enter HELO: ");
    while ((buffer[n++] = getchar()) != '\n')
      ;
    send(sockID,buffer,BUFFER_SIZE,0);
    memset(buffer,0,BUFFER_SIZE);
    recv(sockID,buffer,BUFFER_SIZE,0);
    
    if (strncmp(buffer,"HELO",4) == 0)
        printf("Connection established..\n");
    else{
        perror("Connection not established");
        exit(1);
    } 
    while(1){
        
        n=0;
        printf("MAIL_FROM: ");
        while ((fromBuffer[n++] = getchar()) != '\n');
        send(sockID,fromBuffer,BUFFER_SIZE,0);
        
        n=0;
        printf("RCPT_TO: ");
        while ((toBuffer[n++] = getchar()) != '\n');
        send(sockID,toBuffer,BUFFER_SIZE,0);
        
        n=0;
        printf("DATA: ");
        while ((dataBuffer[n++] = getchar()) != '\n');
        send(sockID,dataBuffer,BUFFER_SIZE,0);
        
        n=0;
        printf("Enter QUIT to exit: ");
        while ((buffer[n++] = getchar()) != '\n');
        if (strncmp(buffer,"QUIT",4) == 0){
            printf("Client Exit..");
            send(sockID,buffer,BUFFER_SIZE,0);
            exit(0);
        }
        
        memset(toBuffer,0,BUFFER_SIZE);
        memset(fromBuffer,0,BUFFER_SIZE);
        memset(dataBuffer,0,BUFFER_SIZE);
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
}
