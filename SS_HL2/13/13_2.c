/*
================================================
Author : Rajan Khade

Description :
Write two programs: first program is waiting to catch SIGSTOP signal, the second program will send the signal (using kill system call). Find out whether the first program is able to catch the signal or not.
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        exit(1);
    }

    pid_t target_pid = atoi(argv[1]);

    // Send the SIGSTOP signal to the target process
    printf("Sending SIGSTOP signal to process with PID = %d\n", target_pid);
    kill(target_pid, SIGSTOP);

    // Optionally, you can send SIGCONT to resume the process after stopping
    sleep(3);  // Wait 3 seconds while the target process is stopped
    printf("Sending SIGCONT signal to resume process with PID = %d\n", target_pid);
    kill(target_pid, SIGCONT);

    return 0;
}
