/*
================================================
Author : Rajan Khade

Description :
Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - Use signal system call.
================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    // Step 1: Ignore SIGINT
    signal(SIGINT, SIG_IGN);
    printf("SIGINT signal is now being ignored. Try pressing Ctrl+C.\n");

    // Sleep for 5 seconds to allow the user to try Ctrl+C
    sleep(5);

    // Step 2: Reset SIGINT to default action
    signal(SIGINT, SIG_DFL);
    printf("\nSIGINT signal restored to default action. Press Ctrl+C again to terminate the program.\n");

    // Wait for a signal
    while (1) {
        printf("Running... Press Ctrl+C to exit.\n");
        sleep(3);
    }

    return 0;
}
