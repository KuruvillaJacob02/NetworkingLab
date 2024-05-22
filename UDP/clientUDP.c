#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

#define BUFFER_SIZE 1024
#define PORT 8080

void main(){
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address;
    int sockID = socket(AF_INET, SOCK_DGRAM, 0);
    int address_size = sizeof(server_address);
    if (sockID == -1){
        perror("Creation of socket failed");
        exit(1);
    }

    printf("Socket creation successful...\n");
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    
    while(1){
        printf("Enter message for Server: ");
        fgets(buffer,BUFFER_SIZE,stdin);
        sendto(sockID,buffer,BUFFER_SIZE, 0,(struct sockaddr*)&server_address,address_size);
        
        memset(buffer,0,BUFFER_SIZE);
        
        recvfrom(sockID,buffer,BUFFER_SIZE,0, (struct sockaddr*)&server_address,&address_size);
        printf("From Server: %s\n",buffer);
        if (strncmp(buffer,"exit",4) == 0 ){
            printf("Client Exits..");
            break;
        }
        
    }
    
    close(sockID);
   
}
