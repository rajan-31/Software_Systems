/*
================================================
Author : Rajan Khade

Description :
Write a program to execute an executable program.
    a. use some executable program
    b. pass some input to an executable program. (for example execute an executable of $./a.out name)
================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    // Command to execute as if entered in a terminal
    char command[50];

    // Prepare the command string
    snprintf(command, sizeof(command), "./test_dir_01/test_prog.out %s", "Test-Input-1");

    // Print the command for verification
    printf("Executing command: %s\n\n", command);

    // Use system() to execute the command
    int result = system(command);

    // Check the result of system()
    if (result == -1) {
        perror("system() failed");
        return 1;
    }

    // If execution was successful, print a message
    printf("Command executed successfully\n");

    return 0;
}
