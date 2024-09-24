/*
================================================
Author : Rajan Khade

Description :
Write a program to print the system resource limits. Use getrlimit system call
================================================
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

// Function to print resource limits
void print_limit(int resource, const char* name) {
    struct rlimit limit;

    // Get the resource limits
    if (getrlimit(resource, &limit) == 0) {
        printf("%-25s  ", name);

        // Check for RLIM_INFINITY (no limit)
        if (limit.rlim_cur == RLIM_INFINITY)
            printf("Soft Limit: Unlimited  ");
        else
            printf("Soft Limit: %ld  ", limit.rlim_cur);

        if (limit.rlim_max == RLIM_INFINITY)
            printf("Hard Limit: Unlimited\n");
        else
            printf("Hard Limit: %ld\n", limit.rlim_max);
    } else {
        perror("getrlimit");
    }
}

int main() {
    printf("System Resource Limits:\n\n");
    printf("%-25s  %-25s  %s\n", "Resource", "Soft Limit", "Hard Limit");

    // Print the limits for various resources
    print_limit(RLIMIT_CPU, "CPU Time (seconds)");
    print_limit(RLIMIT_FSIZE, "File Size (bytes)");
    print_limit(RLIMIT_DATA, "Data Segment Size (bytes)");
    print_limit(RLIMIT_STACK, "Stack Size (bytes)");
    print_limit(RLIMIT_CORE, "Core File Size (bytes)");
    print_limit(RLIMIT_RSS, "Resident Set Size (bytes)");
    print_limit(RLIMIT_NOFILE, "Open File Descriptors");
    print_limit(RLIMIT_AS, "Address Space (bytes)");
    print_limit(RLIMIT_NPROC, "Max Processes");
    print_limit(RLIMIT_MEMLOCK, "Locked Memory (bytes)");
    print_limit(RLIMIT_LOCKS, "File Locks");

    return 0;
}
