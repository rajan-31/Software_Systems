#ifndef IGNORE_IMPORTS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "schema.h"
#endif

void disable_echo_and_buffering() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);          
    tty.c_lflag &= ~(ECHO | ICANON);        
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); 
}

void restore_terminal() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);          
    tty.c_lflag |= (ECHO | ICANON);         
    tcsetattr(STDIN_FILENO, TCSANOW, &tty); 
}

void get_password(char *password) {
    char ch;
    size_t index = 0;

    disable_echo_and_buffering();

    
    while (index < PASSWORD_LEN - 1) {
        ch = getchar();  // Get a character

        // If Enter (newline) is pressed, break the loop
        if (ch == '\n' || ch == '\r') {
            break;
        }

        
        if (ch == 127 || ch == '\b') { 
            if (index > 0) {
                index--;
                printf("\b \b");  
            }
        } else {
            password[index++] = ch; 
            printf("*");
            fflush(stdout);     
        }
    }
    password[index] = '\0'; 

    // Re-enable echo after password input
    restore_terminal();
}

// void getPassword(char *password)
// {
//     static struct termios oldt, newt;
//     int i = 0;
//     int c;

//     /*saving the old settings of STDIN_FILENO and copy settings for resetting*/
//     tcgetattr( STDIN_FILENO, &oldt);
//     newt = oldt;

//     /*setting the approriate bit in the termios struct*/
//     newt.c_lflag &= ~(ECHO);          

//     /*setting the new bits*/
//     tcsetattr( STDIN_FILENO, TCSANOW, &newt);

//     /*reading the password from the console*/
//     while ((c = getchar())!= '\n' && c != EOF && i < 100) {
//         // printf("*");
//         password[i++] = c;
//     }
//     password[i] = '\0';

//     /*resetting our old STDIN_FILENO*/ 
//     tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

// }