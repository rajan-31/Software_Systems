/*
================================================
Author : Rajan Khade

Description :
Write a simple program to print the created thread ids
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Thread function
void* thread_function(void* arg) {
    int thread_num = *(int*)arg;
    pthread_t thread_id = pthread_self();  // Get the thread ID
    printf("Thread %d has ID: %lu\n", thread_num, thread_id);
    pthread_exit(NULL);  // Terminate the thread
}

int main() {
    pthread_t threads[3];  // Array to hold thread IDs
    int thread_nums[3];    // Array to hold the thread arguments

    // Create three threads
    for (int i = 0; i < 3; i++) {
        thread_nums[i] = i + 1;  // Assign an ID to each thread (1, 2, 3)
        if (pthread_create(&threads[i], NULL, thread_function, &thread_nums[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);  // Ensure the main thread waits for the threads to finish
    }

    printf("All threads are finished\n");
    return 0;
}


/*
pthread_self():
This function is used to retrieve the ID of the calling thread. Each thread has a unique ID in the program.

Thread Arguments:
Each thread is passed a unique integer (thread_num), which serves as an identifier for the threads when printing.
*/
