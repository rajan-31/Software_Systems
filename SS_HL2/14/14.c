/*
================================================
Author : Rajan Khade

Description :
Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];  // File descriptors for the pipe (fd[0] for reading, fd[1] for writing)
    char write_msg[] = "Hello!";
    char read_msg[100];

    // Step 1: Create a pipe
    if (pipe(fd) == -1) {
        perror("pipe failed");
        return 1;
    }

    // Step 2: Write data to the pipe
    write(fd[1], write_msg, strlen(write_msg) + 1);  // +1 to include the null terminator
    printf("Written to the pipe: %s\n", write_msg);

    // Step 3: Read data from the pipe
    read(fd[0], read_msg, sizeof(read_msg));
    printf("Read from the pipe: %s\n", read_msg);

    // Step 4: Close the pipe
    close(fd[0]);
    close(fd[1]);

    return 0;
}
