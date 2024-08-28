/*
================================================
Author : Rajan Khade

Description :
Write a program to create three child processes. The parent should wait for a particular child (use waitpid system call).
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;

    // Create first child process
    pid1 = fork();
    if (pid1 < 0) {
        perror("Failed to fork first child");
        exit(1);
    } else if (pid1 == 0) {
        // Inside first child process
        printf("First child process, PID: %d\n", getpid());
        sleep(2);
        exit(0);
    }

    // Create second child process
    pid2 = fork();
    if (pid2 < 0) {
        perror("Failed to fork second child");
        exit(1);
    } else if (pid2 == 0) {
        // Inside second child process
        printf("Second child process, PID: %d\n", getpid());
        sleep(4);
        exit(0);
    }

    // Create third child process
    pid3 = fork();
    if (pid3 < 0) {
        perror("Failed to fork third child");
        exit(1);
    } else if (pid3 == 0) {
        // Inside third child process
        printf("Third child process, PID: %d\n", getpid());
        sleep(6);
        exit(0);
    }

    // In parent process

    // wait for the second child to complete
    if (pid2 > 0) {
        printf("Parent process, waiting for second child PID: %d\n", pid2);
        waitpid(pid2, NULL, 0); // Wait for second child to complete
        printf("Second child has completed\n");
    }

    printf("All children have completed\n");

    return 0;
}
