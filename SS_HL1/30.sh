#!/usr/bin/bash

# /*
# ================================================
# Author : Rajan Khade

# Description :
# Write a program to run a script at a specific time using a Daemon process.
# ================================================
# */

gcc -o time_based_daemon 30.c

sudo ./time_based_daemon /home/rajan/0.Academics/Software_Systems/SS_HL1/test_dir_01/test_script.sh 2 20

tail /var/log/syslog

# sudo kill <pid>
# sudo kill -9 <pid>	# force
