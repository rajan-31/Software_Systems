/*
================================================
Author : Rajan Khade

Description :
Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
	a. access permission
	b. uid, gid
	c. time of last message sent and received
	d. time of last change in the message queue
	d. size of the queue
	f. number of messages in the queue
	g. maximum number of bytes allowed
	h. pid of the msgsnd and msgrcv
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>  // For file operations
#include <unistd.h> // For close()
#include <time.h>   // For time functions

int main() {
    key_t key;
    int msgid;
    struct msqid_ds buf;  // Struct to hold the message queue information
    struct ipc_perm perms;  // Struct to hold the permissions
    const char *filepath = "queuefile";  // File used to generate key with ftok()

    // Check if the file exists, if not create it
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        // File does not exist, create it
        fd = open(filepath, O_CREAT | O_RDONLY, 0666);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        printf("File '%s' created for ftok().\n", filepath);
    }
    close(fd);

    // Generate a key using ftok
    key = ftok(filepath, 65);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the message queue ID using msgget
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Get message queue statistics using msgctl
    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    // Access permissions
    perms = buf.msg_perm;
    printf("Access permissions: %o\n", perms.mode & 0777);  // Use octal representation

    // UID and GID
    printf("UID: %d\n", perms.uid);
    printf("GID: %d\n", perms.gid);

    // Time of last message sent
    printf("Time of last message sent: %s", ctime(&buf.msg_stime));

    // Time of last message received
    printf("Time of last message received: %s", ctime(&buf.msg_rtime));

    // Time of last change in the message queue
    printf("Time of last change in message queue: %s", ctime(&buf.msg_ctime));

    // Size of the queue (in bytes)
    printf("Current size of the queue (bytes): %lu\n", buf.__msg_cbytes);

    // Number of messages in the queue
    printf("Number of messages in queue: %lu\n", buf.msg_qnum);

    // Maximum number of bytes allowed in the queue
    printf("Maximum number of bytes allowed: %lu\n", buf.msg_qbytes);

    // PID of the last msgsnd
    printf("PID of last msgsnd: %d\n", buf.msg_lspid);

    // PID of the last msgrcv
    printf("PID of last msgrcv: %d\n", buf.msg_lrpid);

    return 0;
}
