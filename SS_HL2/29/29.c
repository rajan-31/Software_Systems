/*
================================================
Author : Rajan Khade

Description :
Write a program to remove the message queue
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;

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

    // Remove the message queue using msgctl with IPC_RMID
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl - IPC_RMID");
        exit(EXIT_FAILURE);
    }

    printf("Message queue with ID %d has been removed.\n", msgid);

    return 0;
}
