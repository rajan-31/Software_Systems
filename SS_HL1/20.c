/*
================================================
Author : Rajan Khade

Description :
Find out the priority of your running program. Modify the priority with nice command.
================================================
*/


#include <unistd.h> 
#include <stdio.h>  
#include <stdlib.h> 

void main(int argc, char *argv[])
{
    int priority, newp;
    if (argc != 2)
        printf("1 argument for new priority value is needed!\n");
    else
    {
        newp = atoi(argv[1]);
        priority = nice(0);
        printf("Current priority: %d\n", priority);

        priority = nice(newp);
        printf("New priority: %d\n", priority);
    }
}
