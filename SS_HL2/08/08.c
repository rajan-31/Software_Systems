/*
================================================
Author : Rajan Khade

Description :
Write a separate program using signal system call to catch the following signals.
	a. SIGSEGV
	b. SIGINT
	c. SIGFPE
	d. SIGALRM (use alarm system call)
	e. SIGALRM (use setitimer system call)
	f. SIGVTALRM (use setitimer system call)
	g. SIGPROF (use setitimer system call)
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

// Signal handler function
void handle_signal(int signal) {
    switch (signal) {
        case SIGSEGV:
            printf("Caught SIGSEGV (Segmentation Fault)\n");
            exit(1);
        case SIGINT:
            printf("Caught SIGINT (Interrupt Signal)\n");
            exit(1);
        case SIGFPE:
            printf("Caught SIGFPE (Floating Point Exception)\n");
            exit(1);
        case SIGALRM:
            printf("Caught SIGALRM (Alarm Signal)\n");
            break;
        case SIGVTALRM:
            printf("Caught SIGVTALRM (Virtual Timer Alarm)\n");
            break;
        case SIGPROF:
            printf("Caught SIGPROF (Profiling Timer Alarm)\n");
            break;
        default:
            printf("Caught unexpected signal: %d\n", signal);
            exit(1);
    }
}


// Trigger SIGSEGV (Dereferencing a NULL pointer)
void trigger_seg() {
	int *p = NULL;
	*p = 42;
}

// Trigger SIGFPE (divide by zero)
void trigger_fpe() {
    int a = 1, b = 0;
    int c = a / b;  // This will cause SIGFPE
    (void)c;
}


// Setup setitimer for SIGALRM, SIGVTALRM, and SIGPROF
void setup_timers() {
    struct itimerval timer;

    // SIGALRM (real-time clock)
    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    // SIGVTALRM (virtual time)
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 500000; // 1.5 seconds
    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    // SIGPROF (profiling)
    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;
    setitimer(ITIMER_PROF, &timer, NULL);
}

int main() {
    // Register signal handlers
    signal(SIGSEGV, handle_signal);
    signal(SIGINT, handle_signal);
    signal(SIGFPE, handle_signal);
    signal(SIGALRM, handle_signal);
    signal(SIGVTALRM, handle_signal);
    signal(SIGPROF, handle_signal);

    // Trigger an alarm after 3 seconds using alarm() system call (SIGALRM)
    alarm(3);

    // Set up timers for SIGALRM (ITIMER_REAL), SIGVTALRM, SIGPROF
    setup_timers();

    // trigger_seg();
    // ctrl + c
    trigger_fpe();

    // Infinite loop to keep the program running and catching signals
    while (1) {
        // pause();  // Wait for signals
    }

    return 0;
}
