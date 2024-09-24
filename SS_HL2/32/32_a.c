/*
================================================
Author : Rajan Khade

Description :
Write a program to implement semaphore to protect any critical section.
	a. rewrite the ticket number creation program using semaphore
	b. protect shared memory from concurrent write access
	c. protect multiple pseudo resources ( may be two) using counting semaphore
	d. remove the created semaphore
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_TICKETS 10

sem_t mutex;
int ticket_number = 0;

void* create_ticket(void* arg) {
    for (int i = 0; i < NUM_TICKETS; i++) {
        sem_wait(&mutex); // Enter critical section
        ticket_number++;
        printf("Created ticket number: %d\n", ticket_number);
        sem_post(&mutex); // Exit critical section
        sleep(1); // Simulate some work
    }
    return NULL;
}

int main() {
    pthread_t threads[2];
    sem_init(&mutex, 0, 1); // Initialize semaphore

    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, create_ticket, NULL);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex); // Clean up semaphore
    return 0;
}
