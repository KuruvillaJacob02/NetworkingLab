#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>  //For Fork,exec calls
#include<sys/types.h>
#include<sys/wait.h>
void main(){
    int pid = fork();
    if (pid == -1){
        fprintf(stderr,"Fork Failed");
        exit(1);
    }

    else if (pid == 0){
      printf("This is the child with PID = %d\n",getpid()); //Child Executes here
      char * args[] = {"ls","-l",NULL};
      execvp(args[0],args);
      exit(0);
    }
    
    else{
        int status;
        printf("This is the parent with PID = %d\n",getpid());  //Parent Process Executing
        pid_t child_pid = wait(&status);  //Waits for Child to Finish
        if (WIFEXITED(status)){
            printf("Child %d Exited successfully with status %d",child_pid,WEXITSTATUS(status));
        }
        else
            printf("Child terminated abnormally");   
    }
}
