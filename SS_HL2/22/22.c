/*
================================================
Author : Rajan Khade

Description :
Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>

int main() {
    int fifo_fd;
    fd_set read_fds;
    struct timeval timeout;
    char buffer[256];
    int ret;

    // Create the FIFO if it does not exist
    if (mkfifo("my_fifo", 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Open the FIFO in non-blocking mode
    fifo_fd = open("my_fifo", O_RDONLY | O_NONBLOCK);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Watch stdin (fd 0) to see when it has input
    // Initialize the file descriptor set
    FD_ZERO(&read_fds);
    FD_SET(fifo_fd, &read_fds);

    // Set the timeout
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting for data on FIFO for %d seconds...\n", 10);

    // Wait for data to be available using select()
    ret = select(fifo_fd + 1, &read_fds, NULL, NULL, &timeout);

    if (ret == -1) {
        perror("select");
        close(fifo_fd);
        exit(EXIT_FAILURE);
    } else if (ret == 0) {
        // Timeout reached
        printf("Timeout: No data received within %d seconds.\n", 10);
    } else {
        // Data is available, read from FIFO
        if (FD_ISSET(fifo_fd, &read_fds)) {
            int bytes_read = read(fifo_fd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';  // Null-terminate the string
                printf("Received data: \n%s", buffer);
            } else if (bytes_read == 0) {
                printf("End of FIFO.\n");
            } else {
                perror("read");
            }
        }
    }

    // Clean up
    close(fifo_fd);
    unlink("my_fifo");  // Remove the FIFO file

    return 0;
}
