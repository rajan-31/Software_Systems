#!/usr/bin/bash

# /*
# ================================================
# Author : Rajan Khade
#
# Description :
# Create a FIFO file by
# 	a. mknod command
# 	b. mkfifo command
# 	c. use strace command to find out, which command (mknod or mkfifo) is better.
# 	c. mknod system call
# 	d. mkfifo library function
# ================================================
# */

# p: type of pipe
$ mknod my_fifo1 p

$ mkfifo my_fifo2

$ strace mknod my_fifo3 p

$ strace mkfifo my_fifo4
