/*
================================================
Author : Rajan Khade

Description :
Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
================================================
*/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	char buff[20];

	printf("Enter some input for STDIN, it will be printed immediately after pressing \"enter\" key: \n");

	// 0: stdin, buffer , 11: bytes to read (ignores more than that)
	read(0,buff,10);

	// 1: stdout, buffer, 5: bytes to write (ignores more than that)
	write(1,buff,10);

	return 0;
}
