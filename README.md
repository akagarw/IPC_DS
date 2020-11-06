# Inter-process Communication(IPC) through Various Mediums.
Implementation of IPC using various communication mediums in the linux enviroment in C language, to give general idea how IPC works in Distributed Systems. 
All IPC implemented here is Two-way in nature between Two Processes - Parent-Child, Child-Child or Two Discrete Processes at different Address Spaces.

### Execution
Requirements include: GCC Installed and Linux Environment, to be able to use POSIX Libraries in C language. 

For execution, run the following command in Terminal Window.
```
$ gcc IPC_MediumName.c && ./a.out
```
## Contents/Overview:
- [IPC Using Pipes](https://github.com/akagarw/IPC_DS#ipc-using-pipes) - "IPC_2Way2ChildP.c" and "IPC_2WayPipes.c"
- [IPC Using Named Pipes/FIFOs](https://github.com/akagarw/IPC_DS#ipc-using-named-pipesfifos) - "IPC_FIFO_P1.c" and "IPC_FIFO_P2.c"
- [IPC Using Message Queues]() - "IPC_MQ_P1.c" and "IPC_MQ_P2.c"
- [IPC Using Semaphores]() - "IPC_SetUpSem.c", "IPC_SP_P1.c" and "IPC_SP_P2.c"


## IPC Using Pipes
Pipe is a communication medium between two or more related or interrelated processes. It can be either within one process or a communication between the child and the parent processes. Communication can also be multi-level such as communication between the parent, the child and the grand-child, etc. Communication is achieved by one process writing into the pipe and other reading from the pipe. To achieve the pipe system call, create two files, one to write into the file and another to read from the file.

A Pipe is created by the pipe(int[]) system call with an integer Array as the parameter that is used as the file descriptor(fd) pointing to the created Pipe.
```c
int fd[2];
int f=pipe(fd);
```
```fd[0]``` corresponds to the reading end of the Pipe and hence can be used by the Process to read the messages sent by the other Process.  
```fd[1]``` corresponds to the writing end of the Pipe and hence can be used by the Process to send its messages to the other Process via pipe.


Using ```write()``` and ```read()``` system calls, messages can be written to or read from the pipe by via the buffer character arrays containing the concerned message.
Since Pipes are unidirectional, to support Two Way Communication, two pipes are required, one each for flow of data via both ends P1-P2 and P2-P1.

In the given implementation, constant messages the written by either of the processes. Communication continues until a specific key(The number '0') is pressed. 
- #### Between Parent and Child Process
  As the Program is executed, under the Parent Process, a Child Process is created using the ```fork()``` system call. Based on the return value of ```fork()``` the code sections are separated to be run exclusively by the two processes - One by Child and the other by Parent. Both send their messages via Pipes until the Parent receives input of the specific key. When pressed the parent kills the child process using ```kill()``` call, then exits.
- #### Between 2 Child Processes
  As the Program is executed, under the Parent Process, Two Child Processes are created using the ```fork()``` system call twice. Based on the return value of ```fork()``` the code sections are separated to be run exclusively by the 3 processes. Here, only the children Processes send their messages via Pipes until the Parent receives input of the specific key. When pressed the parent kills both the child processes using ```kill()``` call, then exits.
- ##### References/Documentation
  - Mentioned System Calls/Commands Documentations/Refs [[1](https://man7.org/linux/man-pages/man2/pipe.2.html)] [[2](https://tldp.org/LDP/lpg/node11.html)]
  - [IPC Using Pipes Explanation in Detail](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_pipes.htm)


## IPC Using Named Pipes/FIFOs:
  Pipes can be used for communication between related processes. To achieve unrelated processes communication,i.e. for processes running at different address spaces, one has to use Named Pipes/FIFOs. Using FIFOs we can execute client program from one terminal, the server program from another terminal, then communicate between them.
  
  A Named Pipe is created using the ```mkfifo(fifoFileName,fileMode)``` system call command. This function creates a FIFO special file. The arguments to this function is file name and mode.
  ```c
  char *fifoPipe = "/tmp/fifoFile"; 
  mkfifo(fifoPipe,0666);
  ```
  Using the ```open(filename,mode)``` system call the FIFO special file is opened. Messages are entered by the user via Terminal into a character array buffer, then written to the FIFO file using ```write()``` commands, similarly the messages can be read as well from the file to be displayed on the user Terminal.  
  To simulate the IPC the C Files named - "IPC_FIFO_P1.c" and "IPC_FIFO_P2.c" are executed simultaneously via Terminal, then the messages are exchanged just like a Chatting/Messenging app.   
  The Communication stops when either of the user/Terminal Processes enter the "end" word as the message.

- ##### References/Documentation
  - Mentioned System Calls/Commands Documentations/Refs [[1](https://man7.org/linux/man-pages/man7/fifo.7.html)] [[2](https://tldp.org/LDP/lpg/node15.html)]
  - [IPC using Named Pipes Explanation](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_named_pipes.htm)


## IPC Using Message Queues:
  A message queue is a linked list of messages stored within the kernel and identified by a message queue identifier. All processes can exchange information through access to a common system message queue. The sending process places a message message-passing module) onto a queue which can be read by another process. Each message is given an identification or type so that processes can select the appropriate message. 
  Before proceeding with communication, the Message Queues need to set up with certain IDs. So first an Identifier Key is created by ```ftok()``` command. Using the Generated Key a new queue can be created or an existing queue can be opened using ```msgget()``` command. 
  ```c
  key_t my_key;
  int msg_id;
  
  my_key = ftok("mqFile",3);		
  msg_id = msgget(my_key, 0666 | IPC_CREAT); 	
  ```
  New messages are added to the end of a queue by msgsnd(). Every message has a positive long integer type field, a non-negative length, and the actual data bytes (corresponding to the length), all of which are specified to msgsnd() when the message is added to a queue. Messages are fetched from a queue by msgrcv().  
  To simulate the IPC the C Files named - "IPC_MQ_P1.c" and "IPC_MQ_P2.c" are executed simultaneously via Terminal, then the messages are exchanged just like a Chatting/Messenging app.   
  The Communication stops when either of the user/Terminal Processes enter the "end" word as the message.

  ##### References/Documentation
  - Mentioned System Calls/Commands Documentations/Refs [[1](https://www.man7.org/linux/man-pages/man7/mq_overview.7.html)] [[2](https://tldp.org/LDP/lpg/node27.html)]
  - IPC using Message Queue Explanation [[1](https://www.geeksforgeeks.org/ipc-using-message-queues/)] [[2](https://www.tutorialspoint.com/ipc-using-message-queues)]
   

## IPC Using Semaphores:
  Semaphore in the IPC scenario help to manage such concurrent processes accessing the shared Memory used to communicate.  
  
  Consider the following situation:  
  When multiple processes are using the same region of code and if all want to access parallelly then the outcome is overlapped. Say, for example, multiple users are using one printer only (common/critical section), say 3 users, given 3 jobs at same time, if all the jobs start parallelly, then one user output is overlapped with another. So to prevent overlapping and protect integrity of shared resources(concept of critical section) we need techniques like usage of Semaphores to solve such concurrency issues.  
  
  Semaphore is simply a variable which is non-negative and shared between threads. This variable is used to solve the critical section problem and to achieve process synchronization in the multiprocessing environment. Semaphores simply protects the critical/common region shared among multiple processes. Using Semaphores, locking of the critical section is done when one process is running and unlocking when it is done. This would be repeated for each user/process so that one job is not overlapped with another job.
  
  Before we proceed with the communication, the Semaphores need to be set up with certain IDs(Done in "IPC_SetUpSem.c"). First, an Identifier Key is created by ```ftok()``` command. Using the Generated Key a new Semaphore set can be created or an existing Set can be opened using ```semget()``` command. The semaphore value is then set/initialized to zero(0) using ```semctl()``` command with zero value passed via ```semun``` structure argument and ```SETVAL``` flag.
  ```c
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
  ```
  Now using the Same Semaphore Set Identifier, Semaphore Sets are opened in the Process Files("IPC_SP_P1.c" and "IPC_SP_P2.c") using ```semget()``` for communication. Following Semaphore Operations are first defined as structures that will be passed in the ```semop()``` command to change semaphore values. 
  ```
  struct sembuf semBufWait={0,0,SEM_UNDO};
  struct sembuf semBufLock={0,1,SEM_UNDO | IPC_NOWAIT};
  struct sembuf semBufUnlock={0,-1,SEM_UNDO | IPC_NOWAIT};
  ```
  To communicate here, the messages are written and read via a shared text file opened between the processes. Whichever takes the hold of the file to either read or write, locks the opening of the file by the other process by changing the semaphore state. As one get holds, the process may use ```write()``` and ```read()``` commands to interact with the info in the shared file. 
  ```c
  semop(semid,&semBufWait,1); //Wait for SemValue=0
  semop(semid,&semBufLock,1); //Lock Resource
  
  /* 
    Write or read to shared File
    Basically, Critical Section
  */
  
  semop(semid,&semBufUnlock,1); //Unlock Resource
  ```
  To simulate the IPC the C Files named - First execute "IPC_SetUpSem.c", then "IPC_MQ_P1.c" and "IPC_MQ_P2.c" are executed simultaneously via Terminal. Then the messages can be  exchanged like a Chatting/Messenging app.   
  The Communication stops when either of the user/Terminal Processes enter the "end" word as the message.

  ##### References/Documentation
  - Mentioned System Calls/Commands Documentations/Refs [[1](https://tldp.org/LDP/lpg/node46.html#SECTION00743000000000000000)] [[2](https://man7.org/linux/man-pages/man2/semget.2.html)] [[3](https://users.cs.cf.ac.uk/Dave.Marshall/C/node26.html)]
  - IPC using Message Queue Explanation [[1](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_semaphores.htm)] [[2](https://www.geeksforgeeks.org/semaphores-in-process-synchronization/)]
