/*
================================================
Author : Rajan Khade

Description :
Write a program to create a shared memory.
	a. write some data to the shared memory
	b. attach with O_RDONLY and check whether you are able to overwrite.
	c. detach the shared memory
	d. remove the shared memory
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid;
    char *shmaddr;

    // Get the existing shared memory
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach to shared memory with read-only access
    shmaddr = shmat(shmid, NULL, SHM_RDONLY);
    if (shmaddr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Read and print the current content
    printf("Current content of shared memory: %s\n", shmaddr);

    // Try to overwrite (this should fail)
    printf("Attempting to overwrite read-only shared memory...\n");
    if (strcpy(shmaddr, "This shouldn't work") == NULL) {
        printf("Failed to overwrite read-only shared memory (as expected)\n");
    } else {
        printf("Unexpectedly succeeded in overwriting read-only shared memory\n");
    }

    // Detach from shared memory
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
