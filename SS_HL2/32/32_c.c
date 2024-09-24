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

#define NUM_RESOURCES 2
#define NUM_REQUESTS 5

sem_t resource_semaphore;

void* use_resource(void* arg) {
    for (int i = 0; i < NUM_REQUESTS; i++) {
        sem_wait(&resource_semaphore); // Request resource
        printf("Thread %ld is using a resource.\n", (long)arg);
        sleep(2); // Simulate resource use
        printf("Thread %ld released a resource.\n", (long)arg);
        sem_post(&resource_semaphore); // Release resource
    }
    return NULL;
}

int main() {
    pthread_t threads[3];
    sem_init(&resource_semaphore, 0, NUM_RESOURCES); // Initialize counting semaphore

    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, use_resource, (void*)i);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&resource_semaphore); // Clean up semaphore
    return 0;
}
