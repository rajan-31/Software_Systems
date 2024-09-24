/*
================================================
Author : Rajan Khade

Description :
Write a separate program (for each time domain) to set a interval timer in 10sec and
10micro second
    a. ITIMER_REAL
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for ITIMER_REAL
void real_timer_handler(int signum) {
    printf("ITIMER_REAL signal received (SIGALRM): %d\n", signum);
}

int main() {
    struct itimerval timer;

    // Install signal handler for SIGALRM
    signal(SIGALRM, real_timer_handler);

    // Set the timer interval (10 sec and 10 microseconds)
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    // Set the ITIMER_REAL timer
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("Error setting timer");
        exit(1);
    }

    // Wait for signals
    while (1) {
        pause(); // Waiting for SIGALRM to be caught
    }

    return 0;
}

/*
ITIMER_REAL: Decrements in real time and delivers SIGALRM upon expiration.
ITIMER_VIRTUAL: Decrements when the process is executing in user mode and delivers SIGVTALRM.
ITIMER_PROF: Decrements both when the process is executing in user mode and when the system is executing on behalf of the process. It delivers SIGPROF.


Real Timer (ITIMER_REAL): Sends a SIGALRM signal after the time expires.
Virtual Timer (ITIMER_VIRTUAL): Sends a SIGVTALRM signal when the process executes in user mode.
Profile Timer (ITIMER_PROF): Sends a SIGPROF signal for both user mode execution and system time spent on behalf of the process.
*/
