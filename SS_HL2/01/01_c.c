/*
================================================
Author : Rajan Khade

Description :
Write a separate program (for each time domain) to set a interval timer in 10sec and
10micro second
    c. ITIMER_PROF
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

// Signal handler for ITIMER_PROF
void prof_timer_handler(int signum) {
    printf("ITIMER_PROF signal received (SIGPROF): %d\n", signum);
}

int main() {
    struct itimerval timer;

    // Install signal handler for SIGPROF
    signal(SIGPROF, prof_timer_handler);

    // Set the timer interval (10 sec and 10 microseconds)
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    // Set the ITIMER_PROF timer
    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("Error setting timer");
        exit(1);
    }


    // Simulate user-mode and system-mode CPU activity to trigger the profiling timer
    int fd = open("/dev/null", O_RDONLY);  // File descriptor for /dev/null
    if (fd == -1) {
        perror("Error opening /dev/null");
        exit(1);
    }

    // Wait for signals
    while (1) {
        // pause(); // Waiting for SIGPROF to be caught
        // Busy loop (user mode) + read from /dev/null (system mode)
        for (volatile int i = 0; i < 10000000; i++);  // Busy loop to consume user-mode CPU time
        read(fd, NULL, 0);  // System call to trigger system-mode time (read from /dev/null)
    }

    close(fd);

    return 0;
}
