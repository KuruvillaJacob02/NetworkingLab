#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/time.h>

#define TIMEOUT_SEC 2
#define PORT 8080
#define BUFFER_SIZE 1024

void main(){
    char buffer[BUFFER_SIZE];
    int MAX_FRAMES, WINDOW_SIZE;
    printf("Enter the number of frames to be sent: ");
    scanf("%d",&MAX_FRAMES);
    int base = 1;
    int current_frame = 1;
    struct sockaddr_in server_address;
    int sockID = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockID == -1){
        perror("Socket creation failed");
        exit(1);
    }
    else
        printf("Socket created Successfully..\n");
        
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    
    if (connect(sockID,(struct sockaddr*)&server_address,sizeof(server_address)) != 0){
        perror("Connection Failed..\n");
        exit(1);
    }
    else
        printf("Connected to server..\n");
    
    while (base <= MAX_FRAMES){
        sprintf(buffer,"SEQ%d",current_frame);
        printf("Sending frame : %s\n",buffer);
        send(sockID,buffer,BUFFER_SIZE,0);
        current_frame++;
        
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;
        
        if (setsockopt(sockID, SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)) < 0){
            perror("Setting Timeout failed");
            exit(1);
        }
        
        memset(buffer,0,BUFFER_SIZE);
        int bytes_received = recv(sockID,buffer,BUFFER_SIZE,0);
        
        if (bytes_received > 0){
            int ack_num;
            sscanf(buffer,"ACK%d",&ack_num);
            printf("Acknowledgment received for SEQ%d\n",ack_num);
            if (base == ack_num)
                base++;
                current_frame = base;
        }
        else{
            printf("Timeout or acknowledgment not received. Retransmitting frames in window...\n");
            current_frame = base;
        }
        
    }
    close(sockID);
    exit(0);
}
