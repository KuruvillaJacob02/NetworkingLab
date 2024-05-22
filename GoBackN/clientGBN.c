#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/time.h>    
#include <errno.h>

#define TIMEOUT_SEC 2
#define PORT 8080
#define BUFFER_SIZE 1024

void main(){
    int MAX_FRAMES, WINDOW_SIZE;
    printf("Enter the number of frames to be sent: ");
    scanf("%d",&MAX_FRAMES);
    printf("Enter the window size: ");
    scanf("%d",&WINDOW_SIZE);
    int base = 1;
    int current_frame = 1;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
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
    
    if (connect(sockID, (struct sockaddr*)&server_address, sizeof(server_address)) != 0){
        perror("Failed to connect with server");
        exit(1);
    }
    
    else
        printf("Connected to the server..\n");
    
    while (base <= MAX_FRAMES){  
    
        for (int i = current_frame ; i < base + WINDOW_SIZE && i <= MAX_FRAMES; i++){  //Sending frames to Server
            sprintf(buffer,"SEQ%d",i);
            printf("Sending frame : %s\n",buffer);
            send(sockID,buffer,strlen(buffer),0);
            current_frame++;     
        }
        
        //Set Time out
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SEC;  //Seconds 
        timeout.tv_usec = 0; //Microseconds 
        
        
        if (setsockopt(sockID, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0){
            perror("Setting Timeout failed"); 
            exit(1);
        }
        
        memset(buffer,0, sizeof(buffer)); 
           
        int bytes_received = recv(sockID,buffer,BUFFER_SIZE,0);  // 1 2 3 4 5 6 7 
        
        if (bytes_received > 0){
        
            int ack_num;
            sscanf(buffer, "ACK%d", &ack_num);
            printf("Acknowledgment received for SEQ%d\n",ack_num);
            if (base == ack_num){
                base++;
                current_frame = base+WINDOW_SIZE;  //Slide Window
                if (current_frame >10)
                  current_frame = 10;
            }
        }
        
        else{
            printf("Timeout or acknowledgment not received. Retransmitting frames in window...\n");
            current_frame = base;
        }
        
    }
    
    close(sockID);
    exit(0);
}
