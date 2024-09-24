/*
================================================
Author : Rajan Khade

Description :
Write a separate program using sigaction system call to catch the following signals.
	a. SIGSEGV
	b. SIGINT
	c. SIGFPE
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Signal handler function
void signal_handler(int signal, siginfo_t *siginfo, void *context) {
    switch (signal) {
        case SIGSEGV:
            printf("Caught SIGSEGV (Segmentation Fault). Address causing fault: %p\n", siginfo->si_addr);
            exit(1);  // Exit after handling segmentation fault
        case SIGINT:
            printf("Caught SIGINT (Interrupt Signal). Pressed Ctrl+C.\n");
            exit(1);  // Exit after handling interrupt signal
        case SIGFPE:
            printf("Caught SIGFPE (Floating Point Exception). Error number: %d\n", siginfo->si_errno);
            exit(1);  // Exit after handling floating point exception
        default:
            printf("Caught unknown signal: %d\n", signal);
            exit(1);
    }
}

// Trigger SIGFPE (Floating Point Exception) by dividing by zero
void trigger_fpe() {
    int a = 1, b = 0;
    int c = a / b;  // This will trigger SIGFPE
    (void)c;
}

// Trigger SIGSEGV by accessing invalid memory
void trigger_segv() {
    int *p = NULL;
    *p = 42;  // This will trigger SIGSEGV
}

int main() {
    // Define sigaction structure
    struct sigaction sa;

    // Set the signal handler function
    sa.sa_sigaction = &signal_handler;
    sa.sa_flags = SA_SIGINFO;  // Use the SA_SIGINFO flag to get additional signal info

    // Catch SIGSEGV (Segmentation Fault)
    sigaction(SIGSEGV, &sa, NULL);

    // Catch SIGINT (Interrupt Signal)
    sigaction(SIGINT, &sa, NULL);

    // Catch SIGFPE (Floating Point Exception)
    sigaction(SIGFPE, &sa, NULL);

    // Uncomment one of the following lines to trigger the corresponding signal:

    // Trigger floating point exception
    // trigger_fpe();

    // Trigger segmentation fault
    // trigger_segv();

    // Infinite loop to wait for signals
    while (1) {
        printf("Running... Press Ctrl+C to trigger SIGINT or trigger other signals.\n");
        sleep(3);
    }

    return 0;
}
