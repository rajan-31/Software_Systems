/*
================================================
Author : Rajan Khade

Description :
Write a program to create an orphan process.
================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>

// prent exits before child
int main(){
  int pid  = fork();
  int status;
  if(pid==0){
    printf("Child process is running.\n");
    printf("Child procees id : %d\n",getpid());
    printf("Child Became Orphan.\n");
    sleep(5);
  }
  else{
    printf("Parent process is running\n");
    printf("Parent procees id : %d\n",getpid());
    printf("Parent process exits.\n");
    waitpid(pid, &status, 0);
    exit(0);
  }
  return 0;
}
