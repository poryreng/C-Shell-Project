#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators
#define MAX_PROMPT 256


void cd(char **args);
void clr();
void dir();
void environCom();
void echo(char *argv[]);
void help();
void pauseCom();
void quit();
int otherComm(char **args);
void executeCommands(char **args);
void backgroundExec(char **args, int * background);
void ioRedirect(char **args);

// Student Name: Ben Rory Peng, Student Number 21391003.
// I, Ben Rory Peng acknowledge all of DCU's Academic Integrity Policies.