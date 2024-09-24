/*
================================================
Author : Rajan Khade

Description :
Write a program to create a semaphore and initialize value to the semaphore.
	a. create a binary semaphore
	b. create a counting semaphore
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {
    sem_t *binary_sem;

    // Create a binary semaphore
    binary_sem = sem_open("/binary_semaphore", O_CREAT, 0644, 1);

    if (binary_sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    printf("Binary semaphore created and initialized to 1\n");

    // Close the semaphore
    if (sem_close(binary_sem) == -1) {
        perror("sem_close");
        exit(1);
    }

    return 0;
}
