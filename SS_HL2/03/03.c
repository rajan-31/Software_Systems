/*
================================================
Author : Rajan Khade

Description :
Write a program to set (any one) system resource limit. Use setrlimit system call
================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

// Function to print current limits for a resource
void print_limit(int resource, const char* name) {
    struct rlimit limit;

    if (getrlimit(resource, &limit) == 0) {
        printf("%-25s  Soft Limit: %ld  Hard Limit: %ld\n", name, limit.rlim_cur, limit.rlim_max);
    } else {
        perror("getrlimit");
    }
}

int main() {
    struct rlimit limit;

    printf("Before setting new limits:\n");
    print_limit(RLIMIT_NOFILE, "Open File Descriptors");

    // Set new limits for RLIMIT_NOFILE (open file descriptors)
    limit.rlim_cur = 2048;   // Soft limit (can be changed by unprivileged user)
    limit.rlim_max = 4096;   // Hard limit (can only be changed by root)

    if (setrlimit(RLIMIT_NOFILE, &limit) == 0) {
        printf("Successfully set new limits for open file descriptors.\n");
    } else {
        perror("setrlimit");
        exit(EXIT_FAILURE);
    }

    printf("\nAfter setting new limits:\n");
    print_limit(RLIMIT_NOFILE, "Open File Descriptors");

    return 0;
}


/*
The soft limit is the current enforced limit. Exceeding this value triggers an error or signal.
The hard limit is the maximum value to which the soft limit can be raised. Only privileged users can raise this limit.
*/
