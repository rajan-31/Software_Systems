/*
================================================
Author : Rajan Khade

Description :
Write a simple program to send some data from parent to the child process.
================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int fd[2];  // File descriptors for the pipe (fd[0] for reading, fd[1] for writing)
    pid_t pid;
    char write_msg[] = "Hello from the parent!";
    char read_msg[100];

    // Step 1: Create a pipe
    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // Step 2: Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid > 0) {
        // Parent process
        close(fd[0]);  // Close the read end of the pipe in the parent

        // Step 3: Parent writes data to the pipe
        printf("Parent: Sending message to child...\n");
        write(fd[1], write_msg, strlen(write_msg) + 1);  // +1 to include the null terminator

        close(fd[1]);  // Close the write end after writing
    } else {
        // Child process
        close(fd[1]);  // Close the write end of the pipe in the child

        // Step 4: Child reads data from the pipe
        read(fd[0], read_msg, sizeof(read_msg));
        close(fd[0]);  // Close the read end after reading

        // Step 5: Child displays the received message
        printf("Child: Received message from parent: %s\n", read_msg);
    }

    return 0;
}


/*
The pipe is a unidirectional communication channel: data flows from the writing end to the reading end.
We must close the unused ends of the pipe to avoid resource leaks (e.g., the parent doesn't need the reading end, and the child doesn't need the writing end).
*/
