#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include <errno.h>

#define TIMEOUT_SEC 2
#define PORT 8080
#define BUFFER_SIZE 1024

void main(){
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address, client_address;
    int sockID = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockID == -1){
        perror("Socket creation failed");
        exit(1);
    }
    else
        printf("Socket created Successfully..\n");
        
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    
    if (bind(sockID,(struct sockaddr*)&server_address,sizeof(server_address)) != 0){
        perror("Socket Bind Failed..\n");
        exit(1);
    }
    else
        printf("Socket successfully binded..\n");
    
    if (listen(sockID,5) != 0){
        perror("Listening failed");
        exit(1);
    }
    else{
        printf("Server listening..\n");
    }
    
    int address_size = sizeof(client_address);
    int connectionID = accept(sockID, (struct sockaddr*)&client_address,&address_size);
    
    if (connectionID < 0){
        perror("Server accept failed");
        exit(1);
    }
    
    else
        printf("Server has accepted client..\n");
        
    while (1){
        memset(buffer,0,BUFFER_SIZE);
        int bytes_received = recv(connectionID,buffer,BUFFER_SIZE,0);
        if (bytes_received > 0){
            
            if ((float)rand()/RAND_MAX < 0.3){
                printf("Simulated data loss. Acknowledgment not sent.\n");
                continue;
            }
            
            int seq_num;
            sscanf(buffer,"SEQ%d",&seq_num);
            printf("Sending acknowledgment for frame: SEQ%d\n", seq_num);
            sprintf(buffer,"ACK%d",seq_num);
            send(connectionID,buffer,BUFFER_SIZE,0);
        }
        
        else
        {
            printf("Frame not received from client. Connection may be closed.\n");
            break;
        }    
    }
    
    close(sockID);
    close(connectionID);
    exit(0);
}
