/*
================================================
Author : Rajan Khade

Description :
Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO, SCHED_RR).
================================================
*/

#define _GNU_SOURCE   // Enable GNU-specific features

#include <stdio.h>    // printf, perror
#include <sched.h>    // sched_getscheduler, sched_setscheduler, SCHED_OTHER, SCHED_FIFO, SCHED_RR, sched_param
#include <unistd.h>   // getpid, sleep

void print_scheduler(pid_t pid) {
    int scheduler = sched_getscheduler(pid);
    switch (scheduler) {
        case SCHED_OTHER:
            printf("Current scheduler is SCHED_OTHER\n");
            break;
        case SCHED_FIFO:
            printf("Current scheduler is SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("Current scheduler is SCHED_RR\n");
            break;
        default:
            printf("Unknown scheduler: %d\n", scheduler);
    }
}

int set_scheduler(pid_t pid, int policy) {
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(policy);

    if (sched_setscheduler(pid, policy, &param) == -1) {
        perror("sched_setscheduler");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    pid_t pid = getpid();

    printf("Current scheduling policy:\n");
    print_scheduler(pid);

    printf("\nChanging to SCHED_FIFO...\n");
    if (set_scheduler(pid, SCHED_FIFO) == 0) {
        print_scheduler(pid);
    }

    sleep(2);

    printf("\nChanging to SCHED_RR...\n");
    if (set_scheduler(pid, SCHED_RR) == 0) {
        print_scheduler(pid);
    }

    return 0;
}
