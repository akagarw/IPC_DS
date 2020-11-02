// C program to implement one side of FIFO 
// This side writes first, then reads 
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main() 
{ 
	int fd; 
	char readBuffer[100];
	char userInput[100];
	char *fifoPipe = "/tmp/fifoFile"; 
	mkfifo(fifoPipe,0666); 

	printf("\n  Process 1 running... Enter \"end\" to end the IPC...\n");
	while (1) 
	{ 
		fd = open(fifoPipe, O_WRONLY); 
		printf("\n\tEnter Message : ");
		fgets(userInput, 100, stdin); 
		printf("\t  Sending Message...");
		userInput[strlen(userInput)-1]='\0';
		write(fd, userInput,strlen(userInput)+1); 
		close(fd); 
		if(strcmp(userInput,"end")==0)
		{
			printf("\n  Ending IPC...\n\n");
			break;
		}
		
		fd = open(fifoPipe, O_RDONLY); 
		read(fd,readBuffer,sizeof(readBuffer)); 
		close(fd);
		if(strcmp(readBuffer,"end")==0)
		{
			printf("\n  IPC ended by other Process...\n\n");
			break;
		}
		printf("\n\tMessage Received from Process 2: %s", readBuffer); 
		 
	} 
	return 0; 
} 
