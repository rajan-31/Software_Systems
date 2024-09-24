/*
================================================
Author : Rajan Khade

Description :
Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - use sigaction system call.
================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    // Define a sigaction structure to ignore SIGINT
    struct sigaction sa_ignore;
    sa_ignore.sa_handler = SIG_IGN;  // Ignore SIGINT
    sigemptyset(&sa_ignore.sa_mask);
    sa_ignore.sa_flags = 0;

    // Step 1: Ignore SIGINT
    sigaction(SIGINT, &sa_ignore, NULL);
    printf("SIGINT signal is now being ignored. Try pressing Ctrl+C.\n");

    // Sleep for 5 seconds to allow user to test ignoring SIGINT
    sleep(5);

    // Define a sigaction structure to restore default action of SIGINT
    struct sigaction sa_default;
    sa_default.sa_handler = SIG_DFL;  // Restore default handler for SIGINT
    sigemptyset(&sa_default.sa_mask);
    sa_default.sa_flags = 0;

    // Step 2: Reset SIGINT to default action
    sigaction(SIGINT, &sa_default, NULL);
    printf("\nSIGINT signal restored to default action. Press Ctrl+C again to terminate the program.\n");

    // Infinite loop to keep the program running and allow user to test SIGINT
    while (1) {
        printf("Running... Press Ctrl+C to exit now.\n");
        sleep(3);
    }

    return 0;
}
