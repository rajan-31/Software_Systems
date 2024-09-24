/*
================================================
Author : Rajan Khade

Description :
Write a program to send and receive data from parent to child vice versa. Use two way communication.
================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int pipe1[2];  // Pipe 1: parent to child
    int pipe2[2];  // Pipe 2: child to parent
    pid_t pid;

    char read_buffer[100];

    // Step 1: Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // Step 2: Fork to create a child process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        exit(1);
    }

    if (pid > 0) {
        // Parent process

        char parent_msg[] = "Hello from Parent!";

        // Close unused ends
        close(pipe1[0]);  // Close reading end of pipe1 (parent to child)
        close(pipe2[1]);  // Close writing end of pipe2 (child to parent)

        // Step 3: Parent writes to pipe1 (parent to child)
        write(pipe1[1], parent_msg, strlen(parent_msg) + 1);  // +1 to include null terminator
        printf("Parent: Sent message to child: %s\n\n", parent_msg);

        // Step 4: Parent reads from pipe2 (child to parent)
        read(pipe2[0], read_buffer, sizeof(read_buffer));
        printf("Parent: Received message from child => %s\n", read_buffer);

        // Close pipes
        close(pipe1[1]);
        close(pipe2[0]);

    } else {
        // Child process

        char child_msg[] = "Hello from Child!";

        // Close unused ends
        close(pipe1[1]);  // Close writing end of pipe1 (parent to child)
        close(pipe2[0]);  // Close reading end of pipe2 (child to parent)

        // Step 5: Child reads from pipe1 (parent to child)
        read(pipe1[0], read_buffer, sizeof(read_buffer));
        printf("Child: Received message from parent => %s\n\n", read_buffer);

        // Step 6: Child writes to pipe2 (child to parent)
        write(pipe2[1], child_msg, strlen(child_msg) + 1);  // +1 to include null terminator
        printf("Child: Sent message to parent: %s\n\n", child_msg);

        // Close pipes
        close(pipe1[0]);
        close(pipe2[1]);
    }

    return 0;
}
