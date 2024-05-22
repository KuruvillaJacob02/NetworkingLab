#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

#define BUFFER_SIZE 1024
#define PORT 8080

void main(){
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address, client_address;
    int sockID = socket(AF_INET, SOCK_DGRAM, 0);
    int address_size = sizeof(client_address);
    if (sockID == -1){
        perror("Creation of socket failed");
        exit(1);
    }

    printf("Socket creation successful...\n");
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    
    if (bind(sockID,(struct sockaddr*)&server_address,sizeof(server_address)) != 0){
        perror("Socket bind failed");
        exit(1);
    }
    printf("Socket bind successful..\n");
    
    while(1){
        memset(buffer,0,BUFFER_SIZE);
        recvfrom(sockID,buffer,BUFFER_SIZE,0, (struct sockaddr*)&client_address,&address_size);
        printf("From Client : %s\n",buffer);
        
        printf("Enter message to Client: ");
        fgets(buffer,BUFFER_SIZE,stdin);
        sendto(sockID,buffer,BUFFER_SIZE, 0,(struct sockaddr*)&client_address,address_size);
        if (strncmp(buffer,"exit",4) == 0 ){
            printf("Server Exits..");
            break;
        }
        
    }
    
    close(sockID);
   
}
