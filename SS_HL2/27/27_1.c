/*
================================================
Author : Rajan Khade

Description :
Write a program to receive messages from the message queue.
	a. with 0 as a flag
	b. with IPC_NOWAIT as a flag
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

// Define the structure for a message
struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    // Generate a unique key using ftok
    key = ftok("queuefile", 65);
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
    strcpy(message.msg_text, "This is a test message!");

    // Send the message to the queue using msgsnd
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Message sent to the queue: %s\n", message.msg_text);

    return 0;
}
