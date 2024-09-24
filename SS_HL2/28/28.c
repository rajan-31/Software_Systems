/*
================================================
Author : Rajan Khade

Description :
Write a program to change the exiting message queue permission. (use msqid_ds structure)
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds buf;  // Struct to hold message queue details

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

    // Get the current message queue settings using msgctl with IPC_STAT
    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl - IPC_STAT");
        exit(EXIT_FAILURE);
    }

    // Print current permissions
    printf("Current permissions: %o\n", buf.msg_perm.mode & 0777);

    // Change the permissions (e.g., to 0664)
    buf.msg_perm.mode = (buf.msg_perm.mode & ~0777) | 0664;  // Set new permissions

    // Set the new permissions using msgctl with IPC_SET
    if (msgctl(msgid, IPC_SET, &buf) == -1) {
        perror("msgctl - IPC_SET");
        exit(EXIT_FAILURE);
    }

    // Confirm the change
    printf("Permissions changed to: %o\n", buf.msg_perm.mode & 0777);

    return 0;
}
