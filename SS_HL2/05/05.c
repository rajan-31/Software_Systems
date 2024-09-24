/*
================================================
Author : Rajan Khade

Description :
Write a program to print the system limitation of
	a. maximum length of the arguments to the exec family of functions.
	b. maximum number of simultaneous process per user id.
	c. number of clock ticks (jiffy) per second.
	d. maximum number of open files
	e. size of a page
	f. total number of pages in the physical memory
	g. number of currently available pages in the physical memory.
================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>

// Function to read memory information from /proc/meminfo
long long get_meminfo(const char* key) {
    FILE* file = fopen("/proc/meminfo", "r");
    if (!file) {
        perror("Error opening /proc/meminfo");
        return -1;
    }

    char buffer[256];
    long long value = -1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "%s %lld kB", buffer, &value) == 2 && !strcmp(buffer, key)) {
            fclose(file);
            return value * 1024;  // Convert to bytes
        }
    }

    fclose(file);
    return -1;
}

int main() {
    // a. Maximum length of the arguments to the exec family of functions
    long max_arg_length = sysconf(_SC_ARG_MAX);
    if (max_arg_length == -1) {
        perror("Error retrieving ARG_MAX");
    } else {
        printf("Maximum length of arguments for exec: %ld bytes\n", max_arg_length);
    }

    // b. Maximum number of simultaneous processes per user id
    long max_procs = sysconf(_SC_CHILD_MAX);
    if (max_procs == -1) {
        perror("Error retrieving CHILD_MAX");
    } else {
        printf("Maximum number of simultaneous processes per user ID: %ld\n", max_procs);
    }

    // c. Number of clock ticks (jiffy) per second
    long clock_ticks = sysconf(_SC_CLK_TCK);
    if (clock_ticks == -1) {
        perror("Error retrieving CLK_TCK");
    } else {
        printf("Number of clock ticks per second: %ld\n", clock_ticks);
    }

    // d. Maximum number of open files
    struct rlimit rl;
    if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
        printf("Maximum number of open files: Soft limit = %ld, Hard limit = %ld\n", rl.rlim_cur, rl.rlim_max);
    } else {
        perror("Error retrieving RLIMIT_NOFILE");
    }

    // e. Size of a page
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1) {
        perror("Error retrieving PAGESIZE");
    } else {
        printf("Page size: %ld bytes\n", page_size);
    }

    // f. Total number of pages in physical memory
    long total_pages = sysconf(_SC_PHYS_PAGES);
    if (total_pages == -1) {
        perror("Error retrieving PHYS_PAGES");
    } else {
        printf("Total number of pages in physical memory: %ld\n", total_pages);
    }

    // g. Number of currently available pages in physical memory
    long available_pages = sysconf(_SC_AVPHYS_PAGES);
    if (available_pages == -1) {
        perror("Error retrieving AVPHYS_PAGES");
    } else {
        printf("Number of currently available pages in physical memory: %ld\n", available_pages);
    }

    return 0;
}


/*
Maximum length of arguments for exec: The maximum size (in bytes) of the combined argument list and environment variables passed to an exec call.
Maximum number of processes per user ID: Limits how many processes a user can spawn.
Clock ticks per second: The number of jiffies (ticks) the system counts in one second.
Open file limit: The maximum number of files a process can open (both soft and hard limits).
Page size: The size of a memory page, typically 4096 bytes on most systems.
Total number of pages: Total number of memory pages in the system's physical memory.
Currently available pages: Pages in physical memory currently available for allocation.
*/
