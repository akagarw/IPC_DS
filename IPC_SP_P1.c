/* Communicating with IPC_SP_P2.c using Semaphores */
#include <stdio.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <string.h> 
#define filename "sharedFile.txt"

int main() 
{
	int bytesRead,semid,retval,fd;
	char msgBuf[100]; 
	
	//create unique key for semaphore set
	key_t my_key = ftok(filename,3);
	int nsems = 1;		
	int semflg = IPC_CREAT | IPC_EXCL | 0666;	
	
	//Get Semaphore Set ID using unique Key 
	semid = semget(my_key,nsems,0);
	
	struct sembuf semBufWait={0,0,SEM_UNDO};
	struct sembuf semBufLock={0,1,SEM_UNDO | IPC_NOWAIT};
	struct sembuf semBufUnlock={0,-1,SEM_UNDO | IPC_NOWAIT};
	
	printf("\n  Process 1 running... Enter \"end\" to end the IPC...\n");	
	while (1) 
	{ 
		/** Writing to shared file **/
		semop(semid,&semBufWait,1); //Wait for SemValue=0
		semop(semid,&semBufLock,1); //Lock Resource
		
		printf("\n\tEnter Message : ");
		fgets(msgBuf,100,stdin); 

		printf("\t  Writing Message to the Shared File...");
		msgBuf[strlen(msgBuf)-1]='\0';
		fd = open(filename,O_WRONLY | O_TRUNC);
		write(fd,msgBuf,strlen(msgBuf)); 
		close(fd);
		semop(semid,&semBufUnlock,1); //Unlock Resource
		
		if(strcmp(msgBuf,"end")==0)
		{
			printf("\n  Ending IPC...\n\n");
			break;
		}
		sleep(5);
		
		/** Reading from shared File **/
		semop(semid,&semBufWait,1); //Wait for SemValue=0
		semop(semid,&semBufLock,1); //Lock Resource
		
		fd = open(filename,O_RDONLY);
		bytesRead = read(fd,msgBuf,100); 
		close(fd);
		msgBuf[bytesRead] = '\0'; 

		semop(semid,&semBufUnlock,1); //Unlock Resource
		if(strcmp(msgBuf,"end")==0)
		{
			printf("\n  IPC ended by other Process...\n\n");
			break;
		}		
		printf("\n\tMessage Received from Process 2: %s", msgBuf);
	} 
	
	// destroy the semaphore set
	semctl(semid,0,IPC_RMID);
	return 0; 
} 
