/*
================================================
Author : Rajan Khade

Description :
Write a program, open a file, call fork, and then write to the file by both the child as well as the parent processes. Check output of the file.
================================================
*/


#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include <sys/wait.h>

int main(){
    int fd = open("test_dir_01/test_file_04.txt", O_CREAT | O_RDWR, 0666);
    if(fd == -1) {
        perror("Error opening file");
        return 1;
    }

    int pid = fork();

    if(pid < 0) {
        perror("Error creating child");
    } else if(pid==0) {
        char buff[] = "Written from child process\n";

        int w  = write(fd, &buff, sizeof(buff)-1);
        if(w<0) {
          perror("Cant write using child process\n");
          return 1;
        }
    } else{
        char buff[] = "Written from parent process\n";

        int w  = write(fd,&buff,sizeof(buff)-1);
        if(w<0) {
          perror("Cant write using parent process\n");

          wait(NULL);
          close(fd);

          return 1;
        }

        wait(NULL);
        close(fd);
    }

    return 0;
}
