/*
================================================
Author : Rajan Khade

Description :
Write two programs: first program is waiting to catch SIGSTOP signal, the second program will send the signal (using kill system call). Find out whether the first program is able to catch the signal or not.
================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// Signal handler function for other signals
void signal_handler(int signal) {
    if (signal == SIGINT) {
        printf("Caught SIGINT (Interrupt Signal)\n");
        exit(0);  // Exit when SIGINT is caught
    } else if (signal == SIGCONT) {
        printf("Caught SIGCONT (Continue Signal)\n");
        sleep(5);
        exit(0);
    } else {
        printf("Caught signal: %d\n", signal);
        exit(0);
    }
}

int main() {
    // Register signal handlers
    signal(SIGINT, signal_handler);    // Handle Ctrl+C (SIGINT)
    signal(SIGCONT, signal_handler);   // Handle SIGCONT (resumes process after SIGSTOP)

    printf("Waiting for signals... (PID = %d)\n", getpid());

    // Infinite loop to wait for signals
    while (1) {
        pause();  // Wait for signals
    }

    return 0;
}


/*
The SIGSTOP signal cannot be caught, blocked, or ignored because it is one of the signals that the operating system handles directly. Specifically, SIGSTOP is used to stop (pause) a process, and it can't be caught by the process itself, unlike other signals such as SIGINT or SIGTERM. Only signals like SIGCONT can resume a process that has been stopped by SIGSTOP.


*/
