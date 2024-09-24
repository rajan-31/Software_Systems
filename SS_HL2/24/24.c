/*
================================================
Author : Rajan Khade

Description :
Write a program to create a message queue and print the key and message queue id
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>  // For file operations
#include <unistd.h> // For close()

int main() {
    key_t key;
    int msgid;
    const char *filepath = "queuefile";  // File path for ftok

    // Check if the file exists; if not, create it
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        // File does not exist, create the file
        fd = open(filepath, O_CREAT | O_RDONLY, 0666);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        printf("File '%s' created for ftok().\n", filepath);
    }
    close(fd);

    // Generate a unique key for the message queue
    key = ftok(filepath, 65);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create a message queue and return the message queue ID
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Print the key and the message queue ID
    printf("Message Queue Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}
