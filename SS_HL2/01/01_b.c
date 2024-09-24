/*
================================================
Author : Rajan Khade

Description :
Write a separate program (for each time domain) to set a interval timer in 10sec and
10micro second
    b. ITIMER_VIRTUAL
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for ITIMER_VIRTUAL
void virtual_timer_handler(int signum) {
    printf("ITIMER_VIRTUAL signal received (SIGVTALRM): %d\n", signum);
}

int main() {
    struct itimerval timer;

    // Install signal handler for SIGVTALRM
    signal(SIGVTALRM, virtual_timer_handler);

    // Set the timer interval (10 sec and 10 microseconds)
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    // Set the ITIMER_VIRTUAL timer
    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        perror("Error setting timer");
        exit(1);
    }

    // Wait for signals
    while (1) {
        // pause(); // Waiting for SIGVTALRM to be caught
        for (volatile int i = 0; i < 1000000000; i++);  // Busy loop to consume user-mode CPU time
    }

    return 0;
}

/*
# what "volatile" does and why it might be used:

Prevents optimization: The volatile keyword tells the compiler not to optimize anything about this variable. Without volatile, an optimizing compiler might notice that the loop doesn't do anything and remove it entirely.
Ensures reads and writes: Each read or write to a volatile variable is guaranteed to actually happen as written in the code, without being cached or reordered by the compiler or CPU.
Indicates external changes: It suggests that the variable could be changed by something external to the visible code, like hardware interrupts, a memory-mapped I/O, or another thread in a multi-threaded application.

# Common use cases for volatile:

Memory-mapped I/O registers
Variables shared between interrupt routines and the main program
Variables in multi-threaded applications (though synchronization primitives are generally preferred for this)
*/
