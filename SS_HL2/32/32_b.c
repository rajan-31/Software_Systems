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

#define NUM_WRITES 5

sem_t mutex;
int shared_data = 0;

void* write_to_shared(void* arg) {
    for (int i = 0; i < NUM_WRITES; i++) {
        sem_wait(&mutex); // Enter critical section
        shared_data++;
        printf("Writer %ld wrote: %d\n", (long)arg, shared_data);
        sem_post(&mutex); // Exit critical section
        sleep(1); // Simulate some work
    }
    return NULL;
}

int main() {
    pthread_t threads[3];
    sem_init(&mutex, 0, 1); // Initialize semaphore

    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, write_to_shared, (void*)i);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex); // Clean up semaphore
    return 0;
}
