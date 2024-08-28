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

int main(){
    int fd = open("test_dir_01/test_file_04.txt", O_CREAT | O_RDWR, 0666);
    if(fd==-1) {
        perror("Error opening file");
        return 1;
    }

    int pid = fork();
    if(pid==0){
        char buf[]="Written from Child process\n";
        int wr  = write(fd, &buf, sizeof(buf));
        if(wr<0){
          perror("Cant write using child process\n");
          return 1;
        }
    }
    else{
        char bf[]="Written from parent process\n";
        int wr  = write(fd,&bf,sizeof(bf));
        if(wr<0){
          perror("Cant write using child process\n");
          return 1;
        }
    }
    return 0;
}
