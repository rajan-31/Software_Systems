/*
================================================
Author : Rajan Khade

Description :
Write a program to create a Zombie state of the running program.
================================================
*/

#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <unistd.h>   // fork, getpid, sleep, pid_t


// Child exits before parent
int main() {
    pid_t child_pid = fork(); // Create a child process

    if (child_pid == 0) {
        // child process
        printf("Child process is running.\n");
        printf("Child procees id : %d\n",getpid());
        printf("Child process is exiting.\n");
        exit(0); // Child process exits
    } else if (child_pid > 0) {
        // parent process
        printf("Parent process is running.\n");
        printf("Parent process id: %d\n",getpid());
        sleep(5); // Sleep for 5 seconds to ensure the child becomes a zombie
        printf("Parent process is exiting.\n");
    } else {
        perror("Fork failed");
        return 1; // Fork failed
    }

    return 0;
}

