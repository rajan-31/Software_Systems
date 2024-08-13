/*
================================================
Author : Rajan Khade

Description :
Write a program to display the environmental variable of the user (use environ).
================================================
*/

#include <stdio.h>
#include <stdlib.h>

// This tells the compiler that environ is defined elsewhere (typically by the system), and we want to use it in this program.
extern char **environ;	// Declare the external variable 'environ'

int main() {
	// Pointer to the array of environment variables
	// Initialize a pointer env to point to the start of the environ array.
	char **env = environ;

    for (; *env != NULL; env++) {
        printf("%s\n", *env);
    }
    return 0;
}
