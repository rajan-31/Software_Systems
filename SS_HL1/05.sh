#!/usr/bin/bash

# /*
# ================================================
# Author : Rajan Khade

# Description :
# Write a program to create five new files with infinite loop. Execute the program in the background and check the file descriptor table at /proc/pid/fd.
# ================================================
# */

rm ./test_dir_02/test_file_99?.txt 2> /dev/null

gcc 05.c

./a.out &

pid=$!

echo -e "Proces with pid $pid running in background!\n\n"

sleep 3

echo -e "Output of \"/proc/$pid/fd\" is:\n"
ls -l /proc/$pid/fd

sleep 1

kill $pid
echo -e "\n\nProces with pid $pid killed!"
