#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 

void main(){
	int pid = fork();
	if (pid == -1){
		fprintf(stderr,"Fork Failed\n");	//FORK FAILED
		exit(1);
	}
	
	else if (pid == 0){	//Child Process is Executing
		printf("This is the child and its PID = %d\n",getpid());
		char *args[] = {"ls", NULL}; //	EXEC Command "ls"
        	execvp(args[0], args);
		exit(0);
	}
	
	else if (pid > 1){ 	//Parent Process is Executing
		int status;
		printf("This is a parent and its PID = %d. The child is %d\n",getpid(),pid);
		
		pid_t child_pid;
		child_pid = wait(&status);	//Waits for child to finish executing
		
		if (WIFEXITED(status))		//Check termination status of child process
			printf("Child process with PID %d exited with status: %d\n", child_pid, WEXITSTATUS(status));
		else
			printf("Child process with PID %d terminated abnormally\n", child_pid);
		
	}
}

