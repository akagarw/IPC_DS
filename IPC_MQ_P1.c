/*
Communicating with IPC_MQ_P2.c using Message Queues
*/
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer {
   long msg_type;
   char msg[100];
} message;

int main() 
{ 
	key_t my_key;
	int msg_id;
	
	//create unique key for message queue
	my_key = ftok("mqFile",3);		

	//create message queue using unique key and return id
	// set read write permissions
	msg_id = msgget(my_key, 0666 | IPC_CREAT); 	
	
	printf("\n  Process 1 running... Enter \"end\" to end the IPC...\n");	
	while (1) 
	{ 
		printf("\n\tEnter Message : ");
		fgets(message.msg, 100, stdin); 

		printf("\t  Writing Message to the Queue...");
		message.msg[strlen(message.msg)-1]='\0';
		message.msg_type = 1;
		msgsnd(msg_id, &message, sizeof(message), 0);
		
		if(strcmp(message.msg,"end")==0)
		{
			printf("\n  Ending IPC...\n\n");
			break;
		}
		
		msgrcv(msg_id, &message, sizeof(message),message.msg_type, 0);
		if(strcmp(message.msg,"end")==0)
		{
			printf("\n  IPC ended by other Process...\n\n");
			break;
		}
		printf("\n\tMessage Received from Process 2: %s", message.msg); 

	} 
	// destroy the message queue
	msgctl(msg_id, IPC_RMID, NULL);
	return 0; 
} 
