#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "myshell.h"


extern char **environ;

void cd(char **args){
    if (args[1] == NULL){ //if cd isnt followed by an argument
        char* cwd = getcwd(NULL,0); //gets current directory
        printf("Your current directory: %s\n", cwd); //prints current directory
    }
    else{
        if(chdir(args[1]) != 0){ //if the directory doesn't exist prints error message
            printf("Error:no such directory named: '%s' \n", args[1]);
        }
        else{
            setenv("PWD",getcwd(NULL,0), 1); //sets PWD to current working directory
        }
    }
}

void clr(){
    system("clear"); //clears the screen
}

void dir(){
    system("ls -al");
}

void environCom(){
    int i;
    for (i = 0; environ[i] != NULL; i++) //loops through evironment variables and
        printf("%s\n",environ[i]);      //prints them
}

void echo(char *argv[]){
    for (int i = 1; argv[i]; i++){
        printf("%s ", argv[i]); //prints all characters after the first argument which is "echo"
    }
    printf("\n");
}

void help(){
    system("more ../manual/readme.md");
}

void pauseCom(){
    printf("Shell paused, press Enter to continue");
    getchar();
}

void quit(){
    exit(0);
}

//other commands + implemented background execution
int otherComm(char ** args) {
    int background = 0;
    backgroundExec(args, &background);

    pid_t pid;
    int status;
    pid_t cpid;

    pid = fork();


    if (pid < 0) { //error
        printf("System failed to fork\n");
        return 1;
    }
    else if (pid == 0) { // child process

        if (execvp(args[0], args) < 0) { // execute the command
            printf("Failed to execute command %s\n", args[0]);
            exit(EXIT_FAILURE);
        }
    }

    else { // parent process
        if (background == 0) {
            cpid = waitpid(pid, &status, 0); // wait for child to finish
            if (cpid == -1) {
                printf("Failed to wait for child process\n");
                return 1;
            }
            if (WIFEXITED(status)) {  //to check if the child process ended
                printf("Child process exited with pid: %d\n", WEXITSTATUS(status));
            }
        }
    }
return 0;
}

//function to run the commands in the main()
void executeCommands(char ** args) {
     /*comparing the first argument to the command and calling it*/
        if (!strcmp(args[0], "clr")){
            clr();
        }
        else if (!strcmp(args[0], "environ")){
            environCom();
        }
        else if (!strcmp(args[0], "dir")){
            dir();
        }
        else if (!strcmp(args[0], "cd")){
            cd(args);
        }
        else if (!strcmp(args[0], "echo")){
            echo(args);
        }
        else if (!strcmp(args[0], "help")){
            help();
        }
        else if (!strcmp(args[0], "pause")){
            pauseCom();
        }
        else if (!strcmp(args[0],"quit")){   // "quit" command
            printf("Quitting the shell\n");
            quit();
        }
        else{
            otherComm(args);
        }
    }

//background execution
void backgroundExec(char ** args, int * background) {
    int i = 0;
    while (args[i] != NULL) {
        i++;
    }
    if (i > 0 && strcmp(args[i-1], "&") == 0) {
        *background = 1; //if the & symbol is present the command will be ran in the backgrounfd
        args[i-1] = NULL; // Removes & from the end of the command to run
    } else {
        *background = 0;
    }
}

// void lol(){
//     printf(" \n");
//     printf("       _____                                           _               _      __                      _   _ \n");
//     printf("      /  __ |                                         | |             | |    / _|                    | | | |\n");
//     printf("      | /  -- ___  _ __ ___  _ __ ___   __ _ _ __   __| |  _ __   ___ | |_  | |_ ___  _   _ _ __   __| | | |\n");
//     printf("      | |    / _ || '_ ` _ || '_ ` _ | / _` | '_ | / _` | | '_ | / _ || __| |  _/ _ || | | | '_ | / _` | | |\n");
//     printf("      | |__/| (_) | | | | | | | | | | | (_| | | | | (_| | | | | | (_) | |_  | || (_) | |_| | | | | (_| | |_|\n");
//     printf("      |_____/____/|_| |_| |_|_| |_| |_|__,__|_| |_|__,__| |_| |_|____/|__|  |_| |___/|___,_|_| |_|___,_| (_)\n");
//         printf("\n");
//     printf("                       _.-**-._                         ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣤⣤⣴⣦⣤⣤⣤⣤⣤⣄⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠋⣉⣉⣉⣉⣉⣉⠙⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
//     printf("                    _,(        ),_                      ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⣿⣿⣿⣿⣿⡿⠟⢁⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⡈⠻⢿⣿⣿⣿⣿⣿\n");
//     printf("                 .-\"   '-^----'   \"-.                   ⠀⠀⠀⠀⠀⠀⠀⣤⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⣿⣿⣿⡿⠋⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠙⢿⣿⣿⣿\n");
//     printf("              .-'                    '-.                ⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⣿⣿⡟⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⢻⣿⣿\n");
//     printf("            .'                          '.              ⠀⠀⠀⢀⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⣿⡟⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⢻⣿\n");
//     printf("          .'    __.--**'\"\"\"\"\"\"\"'**--.__    '.           ⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⣿⢀⣿⣿⣿⠟⠁⣠⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣄⠈⠻⣿⣿⣿⡀⣿\n");
//     printf("         /_.-*\"'__.--**'\"\"\"\"\"\"\"'**--.__\"'*-._\\          ⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠋⠉⠉⠁⠀⠀⠀⠀⠀⠀⠈⠙⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⡇⢸⣿⣿⠋⣠⡾⠿⠛⠛⠛⠿⣿⣿⣿⣿⣿⣿⠿⠛⠛⠛⠻⢷⣄⠙⣿⣿⡇⢸\n");
//     printf("        /_..-*\"'   .-*\"*-.  .-*\"*-.   '\"*-.._\\          ⢸⣿⣿⣿⣿⡿⠛⠛⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⡇⢸⣿⣿⣾⣿⢀⣠⣤⣤⣤⣤⣀⣿⣿⣿⣿⣀⣤⣤⣤⣤⣄⡀⣿⣷⣾⣿⡇⢸\n");
//     printf("       :          /       ;:       \\          ;         ⠘⢻⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀  ⡇⠸⠟⣫⣥⣶⣧⠹⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠏⣼⣶⣬⣍⠻⠇⢸\n");
//     printf("       :         :     *  !!  *     :         ;         ⠀⠀⣿⡟⠀⠀⠀⣀⠀⠀⠀⡐⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⣿⠀⠀⠀⠀⠀⠀⠀⠀  ⡧⣰⣿⣿⣿⣿⣿⢰⣦⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣴⡆⣿⣿⣿⣿⣿⣆⢼\n");
//     printf("        \\        '.     .'  '.     .'        /          ⠀⠀⣿⡇⢀⠄⠉⣀⡀⢉⠁⣠⣶⣶⣶⣶⣤⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⣿⣿⣿⣿⣿⡿⢁⠔⠀⠀⠪⢂⠀⠀⠀⠀⠀⠀⠀  ⡇⣿⣿⣿⣿⣿⡟⠈⠙⠛⠻⠿⠿⠿⠿⠿⠿⠿⠿⠟⠛⠋⠁⢻⣿⣿⣿⣿⣿⢸\n");
//     printf("         \\         '-.-'      '-.-'         /           ⠀⠀⠹⡇⢀⣴⣿⡿⢀⠀⠀⠉⠛⠛⠛⠿⣿⣿⣿⣿⣿⣿⣦⠀⡀⠀⠀⠀⠀⠀⢹⣿⣿⣿⡿⠋⠀⠁⠀⠀⠀⠀⢻⠀⠀⠀⠀⠀⠀⠀  ⣿⣌⡻⠿⠿⢋⣴⣦⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⢀⣴⣦⡙⠿⠿⢟⣡⣾\n");
//     printf("      .-*''.                              .'-.          ⠀⠀⢀⣼⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⠀⠀⠀⢉⠡⢀⡀⠀⠀⠀⠀⠀⠸⣿⣿⡟⠀⠀⠀⠐⠂⠀⠀⡠⠀⠀⠀⠀⠀⠀⠀⠀  ⣿⣿⣿⣷⣄⠙⢿⣿⣿⣶⣤⣀⡀⡀⡀⡀⡀⡀⣀⣤⣶⣿⣿⡿⠋⣠⣾⣿⣿⣿\n");
//     printf("   .-'      '.                          .'    '.        ⠀⠀⣾⣿⡿⠛⠐⠂⠤⡐⠄⠀⢠⠀⠀⠊⠶⠛⠛⠛⠊⠥⠄⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠇⢸⠀⠀⠀⠀⠀⠀⠀  ⣿⣿⣿⣿⣿⣷⣦⣉⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⣉⣴⣾⣿⣿⣿⣿⣿\n");
//     printf("  :           '-.        _.._        .-'        '._     ⠀⠀⢹⡏⠀⣀⣴⠶⠶⢾⠰⠀⠀⠀⠀⠀⠠⠤⠄⠂⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠘⠊⢸⠀⠀⠀⠀⠀⠀⠀  ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣌⣉⣉⣉⣉⣉⣉⣡⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
//     printf(";\"*-._          '-._  --___ `   _.-'        _.*'  '*.   ⠀⠀⢹⡏⠀⣀⣴⠶⠶⢾⠰⠀⠀⠀⠀⠀⠠⠤⠄⠂⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠘⠊⢸⠀⠀⠀⠀⠀⠀⠀\n");
//     printf(":      '.            '\"*-.__.-*\"'           (        :  ⠀⠀⠀⠣⡞⢟⠁⢀⠠⢘⡼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠁⠀⠀⢀⠆⠀⠀⠀⠀⠀⠀⠀  ───█───▄▀█▀▀█▀▄▄───▐█──────▄▀█▀▀█▀▄▄\n");
//     printf(" ;      ;                 *|                 '-.     ;  ⠀⠀⠀⠀⡹⠂⠀⠀⡐⠁⠆⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⠃⠀⠀⠀⠀⠀⠀⠀⠀  ──█───▀─▐▌──▐▌─▀▀──▐█─────▀─▐▌──▐▌─█▀\n");
//     printf("  '---*'                   |                    \"\"--'   ⠀⠀⠀⢠⠁⠀⠀⡌⠀⡘⠀⠀⠀⠀⠀⠀⠀⢱⠀⠀⠑⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⢸⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀  ─▐▌──────▀▄▄▀──────▐█▄▄──────▀▄▄▀──▐▌\n");
//     printf("   :                      *|                      :     ⠀⠀⠀⠈⠀⠀⢰⠀⢈⢃⡀⠀⢠⠐⠀⠀⠀⠁⠀⠀⠀⠀⠑⠄⠀⠀⠀⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⣼⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀  ─█────────────────────▀█────────────█\n");
//     printf("   '.                      |                     .'     ⠀⠀⠀⠘⡀⠀⠀⠄⠀⠉⠁⠀⠀⠀⠀⣀⠠⠄⢀⣀⣤⣀⠀⠈⡄⡄⠀⢡⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⠀⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀  ▐█─────────────────────█▌───────────█\n");
//     printf("     '.._                 *|        ____----.._-'       ⠀⠀⠀⠀⠐⡄⠀⡀⠀⢀⠤⠒⣈⣩⡤⢶⠞⠟⢙⠉⡋⣛⣿⡆⠀⡇⠀⡈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ▐█─────────────────────█▌───────────█\n");
//     printf("      \\  \"\"\"----_____------'-----\"\"\"         /          ⠀⠀⠀⠀⠀⠈⢄⠐⢄⠠⣿⣿⡋⡁⢁⣀⣤⣷⣾⣿⣿⣿⢿⠃⠀⡇⢠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ─█───────────────█▄───▄█────────────█\n");
//     printf("       :'\"              '-..--''          \";            ⠀⠀⠀⠀⠀⠀⠈⠢⡀⠑⡜⢜⠻⣿⣿⡿⢻⣻⠹⠕⠋⢁⠊⠀⠀⠇⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ─▐▌───────────────▀███▀────────────▐▌\n");
//     printf("        --------------------|---------------            ⠀⠀⠀⠀⠀⠀⠀⠀⠈⠢⠈⢄⠢⢀⣉⡁⠁⠀⠤⠐⠊⠀⠀⠀⠠⠰⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡠⠂⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ──█──────────▀▄───────────▄▀───────█\n");
//     printf("                                                        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⡈⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠔⢁⠁⠀⠀⠀⠀⠀⠀⠀⠀⢀⠔⠀⠀⠀⠠⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀  ───█───────────▀▄▄▄▄▄▄▄▄▄▀────────█\n");
//     printf("                                                        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠃⠀⠀⠀⠀⠀⠘⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
//     printf("                                                        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢣⢀⠀⠀⠀⠀⠀⠀⠀\n");
//     printf("                                                        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⢄⠀⠀⠀⠀⠀⢀⠠⠔⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡄⠱⡀⠀⠀⠀⠀⠀\n");
//     printf("                                                        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡰⠫⡁⠒⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⡄⠈⠐⠠⡀⠀⠀\n");
//     printf("                                                        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡔⠀⠀⢡⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡅⠀⠀⠀⠈⠰⡀\n");
//     printf("                                                        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠀⠀⠀⠀⠡⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀\n");
//     printf("                                                        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀\n");

// }


//IO REDIRECTION NOT WORKING
// void ioRedirect(char **args){
//     if (args[1] != NULL && strcmp(args[1], "<") == 0){
//         int fd = open(args[2]);
//         if (fd < 0){
//             printf("Failed to open input file: '%s\n'", args[2]);
//             exit(EXIT_FAILURE);
//         }
//         dup2(fd, STDIN_FILENO);
//     }

//     if (args[1] != NULL && strcmp(args[1], ">") == 0){
//         int fd = open(args[2]);
//         if (fd < 0){
//             printf("Failed to open output file %s\n", args[2]);
//             exit(EXIT_FAILURE);

//         }

//     }
// }


// Student Name: Ben Rory Peng, Student Number 21391003.
// I, Ben Rory Peng acknowledge all of DCU's Academic Integrity Policies.