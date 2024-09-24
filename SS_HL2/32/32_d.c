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

sem_t mutex;

void* do_something(void* arg) {
    sem_wait(&mutex); // Enter critical section
    printf("Thread %ld is doing something.\n", (long)arg);
    sleep(1); // Simulate some work
    printf("Thread %ld finished.\n", (long)arg);
    sem_post(&mutex); // Exit critical section
    return NULL;
}

int main() {
    pthread_t threads[2];
    sem_init(&mutex, 0, 1); // Initialize semaphore

    for (long i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, do_something, (void*)i);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex); // Clean up semaphore
    return 0;
}
