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
    int pipePtoC[2],pipeCtoP[2]; 
    
    if (pipe(pipePtoC)==-1) 
    { 
        fprintf(stderr, "Pipe Creation Failed" ); 
        return; 
    } 
    if (pipe(pipeCtoP)==-1) 
    { 
        fprintf(stderr, "Pipe Creation Failed" );
        return; 
    } 
            
    int flag=1;
    int messageCtr=0;
    int childpid = fork();     
    
    if(childpid == 0) 
    {
        //Child 1 running here      
        while(1)
        {
            messageCtr++;
            char message[100];
            sprintf(message,"Message %d from Child : Hello!",messageCtr);     
            
            close(pipeCtoP[0]);
            printf("\n  Child - Writing to Pipe."); 
            write(pipeCtoP[1],message, strlen(message)+1); 
            
            close(pipePtoC[1]);
            read(pipePtoC[0], readBuffer, MSGSIZE);
            printf("\n  Child - Reading from Pipe...");
            printf("\n\t%s", readBuffer);
            sleep(2);
        }
    }

    if(childpid>0)
    { 
        //Parent Process running here

        printf("\n    Child created! ( Pid: %d )",childpid);

        while(1)
        {
            messageCtr++;
            char message[100];
            sprintf(message,"Message %d from Parent : Hello!",messageCtr);     
            
            close(pipePtoC[0]);
            printf("\n  Parent - Writing to Pipe.");
            write(pipePtoC[1],message, strlen(message)+1); 
            
            close(pipeCtoP[1]);
            read(pipeCtoP[0], readBuffer, MSGSIZE);
            printf("\n  Parent - Reading from Pipe...");
            printf("\n\t%s", readBuffer);
            //sleep(2);

            scanf("%d",&flag);
            if(flag==0)
            {
                kill(childpid, SIGKILL);
                break;
            }
        } 
    } 
}  
