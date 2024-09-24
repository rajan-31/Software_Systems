/*
================================================
Author : Rajan Khade

Description :
Write a simple program to create three threads.
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Thread function
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    printf("Thread %d is running\n", thread_id);
    sleep(1);  // Simulate some work with sleep
    printf("Thread %d is finishing\n", thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[3];  // Array to hold thread IDs
    int thread_ids[3];     // Array to hold the thread arguments

    // Create three threads
    for (int i = 0; i < 3; i++) {
        thread_ids[i] = i + 1;  // Assign an ID to each thread
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
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
pthread_t: This is the data type used for thread identifiers. Here, we declare an array threads[3] to store the IDs of the three threads.

pthread_create(): This function creates a new thread. It takes four arguments:

&threads[i]: Pointer to the thread ID.
    NULL: The thread's attributes (set to default).
    thread_function: The function that the thread will execute.
    &thread_ids[i]: Pointer to an argument that will be passed to the thread function.
    Thread Function: Each thread runs the function thread_function(), which takes a void* argument (in this case, an integer representing the thread ID). It prints a message, sleeps for 1 second, and then exits.

pthread_join(): This ensures the main program waits for each thread to finish before exiting. Without this, the main thread could exit before the created threads complete their execution.
*/
