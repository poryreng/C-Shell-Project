#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "myshell.h"
#include "utility.c"

int main(int argc, char ** argv){
    char buf[MAX_BUFFER]; //line buffer
    char *args[MAX_ARGS]; //pointer to arg strings
    char ** arg; //working pointer through args
    char prompt[MAX_PROMPT]; //shell prompt

    /*Batch Mode*/
    if (argv[1] != 0){ //checking if the file exists
        FILE* fp = fopen(argv[1], "r");
        if (fp == NULL){ //if file cant be opened, error message is printed
            fprintf(stderr, "Error: could not open file\n");
            exit(EXIT_FAILURE);
        }
        while (fgets(buf, MAX_BUFFER, fp) != NULL){ //reads each line of file until the end of the file is reached

         /*tokenize each line of file into args array*/
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);
            while ((*arg++ = strtok(NULL, SEPARATORS)))
            ;
                if (args[0]) {
                    executeCommands(args); //commands are executed
                }
        }
        fclose(fp); //exits file
        exit(EXIT_SUCCESS);
    }


    /*shell inputs*/
    while (!feof(stdin)){
        char cwd[MAX_PROMPT];
        getcwd(cwd, sizeof(cwd));
        sprintf(prompt, "%s/>> ", cwd);
        fputs(prompt, stdout); //gets command line from input

        if(fgets (buf, MAX_BUFFER, stdin)){
            /*tokenize the input into args array*/
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);

            while ((*arg++ = strtok(NULL, SEPARATORS))); //last entry will be NULL
            if (args[0]){
                executeCommands(args); //commands are executed
            }
        }
    }
    return 0;
}


// Student Name: Ben Rory Peng, Student Number 21391003.
// I, Ben Rory Peng acknowledge all of DCU's Academic Integrity Policies.