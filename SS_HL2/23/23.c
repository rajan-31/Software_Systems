/*
================================================
Author : Rajan Khade

Description :
Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer)
================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int main() {
    int max_files = sysconf(_SC_OPEN_MAX);

    printf("Maximum number of files that can be opened within a process: %d\n", max_files);
    printf("Size of a pipe (circular buffer): %d bytes\n", PIPE_BUF);

    return 0;
}
