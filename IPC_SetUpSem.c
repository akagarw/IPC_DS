/* Set Up Semaphores for IPC */
#include <stdio.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <string.h> 
#define filename "sharedFile.txt"

union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
	struct seminfo  *__buf;
} arg;

int main() 
{
	//Get the unique key for semaphore set
	int semid;
    key_t my_key = ftok(filename,3);
	int semflg = IPC_CREAT | IPC_EXCL | 0666;
	int nsems = 1;		

    // Get Semaphore Set ID if created or existing one
    if( (semid = semget(my_key,nsems,semflg)) == -1)
		semid = semget(my_key,1,0);

	arg.val=0;
    semctl(semid,0,SETVAL,arg);

    int semVal=semctl(semid,0,GETVAL);

    printf("\n  Setting Up Semaphore Set... ");
    printf("Done!");
    printf("\n\tInitialized to Value : %d\n\n",semVal);

    return 0;
}