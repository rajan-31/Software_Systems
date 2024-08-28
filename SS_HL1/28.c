/*
================================================
Author : Rajan Khade

Description :
Write a program to get maximum and minimum real time priority.
================================================
*/


#include <stdio.h>
#include <sched.h>  // sched_get_priority_max, sched_get_priority_min, SCHED_FIFO

int main() {
    // SCHED_FIFO is a scheduling policy used in real-time systems for processes or threads that require strict scheduling guarantees

    /* ============================ */

    int maxx = sched_get_priority_max(SCHED_FIFO);
    if (maxx == -1) {
        perror("Error getting max real time priority!\n");
        return 1;
    }
    printf("Max real time priority: %d\n", maxx);

    /* ============================ */

    int minn = sched_get_priority_min(SCHED_FIFO);
    if (minn == -1) {
        perror("Error getting min real time priority!\n");
        return 1;
    }
    printf("Min real time priority: %d\n", minn);

    return 0;
}
