/*
================================================
Author : Rajan Khade

Description :
Write a program to find out time taken to execute getpid system call. Use time stamp counter.
================================================
*/


#include <stdio.h>     // printf
#include <sys/types.h> // pid_t
#include <unistd.h>    // getpid
#include <time.h>      // clock, CLOCKS_PER_SEC


int main(){
clock_t t;

t = clock();
    double an = (double)t/CLOCKS_PER_SEC;
    printf("Start time is %f\n",an);

    pid_t pid = getpid();

    t = clock();
    double ans = (double)t/CLOCKS_PER_SEC;
    printf("End time is : %f\n",ans);

    double f = ans-an;
    printf("\nTotal Time Taken is %f\n",f);

    return 0;
}

