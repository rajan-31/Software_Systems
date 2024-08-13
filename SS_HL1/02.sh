#!/usr/bin/bash

# /*
# ================================================
# Author : Rajan Khade

# Description :
# Write a simple program to execute in an infinite loop at the background. Go to /proc directory and identify all the process related information in the corresponding proc directory.
# ================================================
# */

gcc 02.c && ./a.out &

pid=$!

echo -e "Proces with pid $pid running in background!\n\n"

# cat /proc/$pid/status
cat /proc/$pid/limits

sleep 1

kill $pid
echo -e "\n\nProces with pid $pid killed!"
