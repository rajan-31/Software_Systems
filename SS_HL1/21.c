/*
================================================
Author : Rajan Khade

Description :
Write a program, call fork and print the parent and child process id.
================================================
*/


#include <stdio.h>     // printf
#include <unistd.h>    // fork, getpid


int main(){
    int pid;

    pid = fork();

    if(pid==0)
        printf("Child process id: %d\n",getpid());
    else
        printf("Parent process id: %d\n",getpid());

    return 0;
}
