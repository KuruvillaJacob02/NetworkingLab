#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

int main(int args, char *argv[]){
	int source, dest;
	ssize_t bytes_read;
	char buffer[BUFFER_SIZE];
	source = open(argv[1], O_RDONLY); //Open source in Read only mode
	
	if (source == -1){
		perror("Error opening source file\n");
		return 1;
	}
	
	dest = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC ,0644); 	//Write only, Create if does not exist, Delete contents of file
	if (dest == -1) {
       	perror("Error opening destination file");
        	close(source);
        	return 1;
    	}
    	
    	while ((bytes_read = read(source, buffer, BUFFER_SIZE)) > 0) {
    		write(dest, buffer, bytes_read);
    	}
    	
    	if (bytes_read == -1) {
        	perror("Error reading from source file");
        	close(source);
    		close(dest);
    		return 1;
        }
        
        close(source);
        close(dest);
    	printf("File copied successfully!\n");
    	return 0;
    	
}
