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
    sem_t *counting_sem;
    int initial_value = 5;  // You can change this to any positive integer

    // Create a counting semaphore
    counting_sem = sem_open("/counting_semaphore", O_CREAT, 0644, initial_value);

    if (counting_sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    printf("Counting semaphore created and initialized to %d\n", initial_value);

    // Close the semaphore
    if (sem_close(counting_sem) == -1) {
        perror("sem_close");
        exit(1);
    }

    return 0;
}
