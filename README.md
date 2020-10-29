# Inter-process Communication(IPC) through Various Mediums.
Implementation of IPC using various communication mediums in the linux enviroment in C language, to give general idea how IPC works in Distributed Systems. 
All IPC implemented here is Two-way in nature between Two Processes - Parent-Child, Child-Child or Two Discrete Processes at different Address Spaces.

### Execution
Requirements include: GCC Installed and Linux Environment, to be able to use POSIX Libraries in C language. 

For execution, run the following command in Terminal Window.
```
$ gcc IPC_Medium.c && ./a.out
```
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
  - [Man7 Org - Pipe() call documentation](https://man7.org/linux/man-pages/man2/pipe.2.html)  
  - [TLDP Org - Pipe() call documentation](https://tldp.org/LDP/lpg/node11.html)  
  - [IPC Using Pipes Explanation in Detail](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_pipes.htm)

