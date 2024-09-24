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

// Define the structure for a message
struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    // Generate a key using ftok (create file queuefile if necessary)
    key = ftok("queuefile", 65);
    if (key == -1) {
        // If queuefile does not exist, create it
        FILE *fp = fopen("queuefile", "w");
        if (fp == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fclose(fp);

        // Retry generating the key
        key = ftok("queuefile", 65);
        if (key == -1) {
            perror("ftok");
            exit(EXIT_FAILURE);
        }
    }

    // Get the message queue ID using msgget
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Try receiving a message with the IPC_NOWAIT flag
    if (msgrcv(msgid, &message, sizeof(message.msg_text), 0, IPC_NOWAIT) == -1) {
        perror("msgrcv");
        printf("No message available (non-blocking call).\n");
        exit(EXIT_FAILURE);
    }

    // Print the received message
    printf("Received message: %s\n", message.msg_text);

    return 0;
}
