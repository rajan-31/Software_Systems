/*
================================================
Author : Rajan Khade

Description :
Write a program to execute ls -Rl by the following system calls
    a. execl
    b. execlp
    c. execle
    d. execv
    e. execvp
================================================
*/


#include <stdio.h>      // printf, perror
#include <unistd.h>     // fork, execl, execlp, execle, execv, execvp, _exit, NULL
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    char *args[] = {"ls", "-Rl", NULL};
    char *envp[] = {NULL};

    // Using execl
    pid = fork();
    if (pid == 0) {
        // Child process
        printf("=========Executing \"ls -Rl\" by \"execl\"=========\n");
        execl("/bin/ls", "ls", "-Rl", NULL);
        perror("execl failed");
        return 1;
    } else {
        // Parent process waits for child to finish
        wait(NULL);
    }

    // Using execlp
    pid = fork();
    if (pid == 0) {
        printf("\n=========Executing \"ls -Rl\" by \"execlp\"=========\n");
        execlp("ls", "ls", "-Rl", NULL);
        perror("execlp failed");
        return 1;
    } else {
        wait(NULL);
    }

    // Using execle
    pid = fork();
    if (pid == 0) {
        printf("\n=========Executing \"ls -Rl\" by \"execle\"=========\n");
        execle("/bin/ls", "ls", "-Rl", NULL, envp);
        perror("execle failed");
        return 1;
    } else {
        wait(NULL);
    }

    // Using execv
    pid = fork();
    if (pid == 0) {
        printf("\n=========Executing \"ls -Rl\" by \"execv\"=========\n");
        execv("/bin/ls", args);
        perror("execv failed");
        return 1;
    } else {
        wait(NULL);
    }

    // Using execvp
    pid = fork();
    if (pid == 0) {
        printf("\n=========Executing \"ls -Rl\" by \"execvp\"=========\n");
        execvp("ls", args);
        perror("execvp failed");
        return 1;
    } else {
        wait(NULL);
    }

    return 0;
}
