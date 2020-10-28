#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#define MSGSIZE 100 

void main() 
{ 
	printf("\nEnter 0 to terminate the communication...\n");
    printf("\n  Parent Process running...");

    char readBuffer[MSGSIZE]; 
    int pipeC1toC2[2],pipeC2toC1[2]; 
    
    if (pipe(pipeC1toC2)==-1) 
    { 
        fprintf(stderr, "Pipe Creation Failed" ); 
        return; 
    } 
    if (pipe(pipeC2toC1)==-1) 
    { 
        fprintf(stderr, "Pipe Creation Failed" );
        return; 
    } 
		   	
	int flag=1;
	int messageCtr=0;
	int child1pid = fork(); 	
	
	if(child1pid == 0) 
    {
    	//Child 1 running here    	
    	while(1)
    	{
    		messageCtr++;
		    char message[100];
		    sprintf(message,"Message %d from Child 1 : Hello!",messageCtr);		
			
	    	close(pipeC1toC2[0]);
			printf("\n  Child 1 - Writing to Pipe."); 
			write(pipeC1toC2[1],message, strlen(message)+1); 
			
			close(pipeC2toC1[1]);
			read(pipeC2toC1[0], readBuffer, MSGSIZE);
			printf("\n  Child 1 - Reading from Pipe...");
			printf("\n\t%s", readBuffer);
			sleep(2);
    	}
    }

    if(child1pid>0)
    { 
    	printf("\n    Child 1 created! ( Pid: %d )",child1pid);

    	//Forking Child 2 in parent process
        int child2pid = fork();

        if (child2pid == 0) 
	    { 
	    	//Child 2 Process Running here
	        while(1)
	        {
	        	messageCtr++;
			    char message[100];
			    sprintf(message,"Message %d from Child 2 : Hello!",messageCtr);		
				
		        close(pipeC2toC1[0]);
				printf("\n  Child 2 - Writing to Pipe.");
	 		    write(pipeC2toC1[1],message, strlen(message)+1); 
				
				close(pipeC1toC2[1]);
				read(pipeC1toC2[0], readBuffer, MSGSIZE);
				printf("\n  Child 2 - Reading from Pipe...");
				printf("\n\t%s", readBuffer);
				sleep(2);
	        }
	    }

	    //Parent Process running here with both Children existing
	    if(child2pid > 0)
	    {
	    	printf("\n    Child 2 created! ( Pid: %d )",child2pid);
	    	while(1)
		  	{
		  		scanf("%d",&flag);
		  		if(flag==0)
		  		{
	                kill(child1pid, SIGKILL);
	                kill(child2pid, SIGKILL);
	                break;
	            }
		  	} 
	    }	
    } 
}  
