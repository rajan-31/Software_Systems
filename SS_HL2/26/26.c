/*
================================================
Author : Rajan Khade

Description :
Write a program to send messages to the message queue. Check $ipcs -q
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>  // For file operations
#include <unistd.h> // For close()

// Define the structure for a message
struct msg_buffer {
    long msg_type;       // Message type (must be > 0)
    char msg_text[100];  // Message text content
};

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

    struct msg_buffer message;

    // Generate a unique key using ftok
    key = ftok(filepath, 65);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the message queue ID using msgget, or create it if it doesn't exist
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Prepare the message to send
    message.msg_type = 1;  // Message type, must be a positive integer
    strcpy(message.msg_text, "Hello, this is a test message!");

    // Send the message to the queue using msgsnd
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Message sent to the queue: %s\n", message.msg_text);

    return 0;
}
