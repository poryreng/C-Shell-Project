# My C Shell Manual:


### Name: My Shell

## Description:
Myshell is a simple command line shell that supports the execution of various commands such as the ones listed below. The shell also supports background execution of external commands and batchfile.


## INTERNAL COMMANDS:

    1. cd directory - changes the current directory to the given directory from the argument, if there's no argument given, cd will print out the current working directory

    2. clr - clears the screen

    3. dir - lists all files and permission, ownership and modifiaction time along with it

    4. environ - lists all enviroment strings

    5. echo string - outputs the argument given onto the command line

    6. pause - pauses the operation until the Enter key is pressed

    7. quit - quits the shell

## Enviroment Concepts:
The shell enviroment is a set of variables that affect the behaviour of the shell and its commands. The eviroment variables can be accessed and changed by the user. The variables can be also used to control the  behaviour of shell scripts and other programs. We can see that in this MyShell, when cd is used the PWD enviroment variable is updated to the current directory of the shell.

## I/O Redirection:
Input Output Redirection allows us to change the standard input and out put when executing a command on the terminal, The standard I/O is overwritten with contents in the file. The < symbol is used to overwrite the standard input. The > symbol is used to overwrite standard output.

## Process Concept and Background/Foreground Execution:
A command can be ran in the background by typing "&" after the command. You can check the process status of the command by typing 'ps'. The internal quit command quits all active proccesses. When a process is forked, it creates a new process (the child process) that is an exact copy of the original process (the parent process) at the point of the fork. The child process can then run independently of the parent process.

## References
Graham Healy, Ian G Graham - [Your first shell][https://loop.dcu.ie/mod/book/view.php?id=2054177&chapterid=433263]

IBM - [Query status to see if a child process ended normally][https://www.ibm.com/docs/en/ztpf/1.1.0.15?topic=zca-wifexitedquery-status-see-if-child-process-ended-normally]

GeeksforGeeks - [fork() in C][https://www.geeksforgeeks.org/fork-system-call/]

Smriti M - [Running background process in C using fork()][https://www.experts-exchange.com/questions/29063374/Running-background-process-in-C-using-fork.html]

Student Name: Ben Rory Peng, Student Number 21391003.
I, Ben Rory Peng acknowledge all of DCU's Academic Integrity Policies.