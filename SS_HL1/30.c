/*
================================================
Author : Rajan Khade

Description :
Write a program to run a script at a specific time using a Daemon process.
================================================
*/


#include <stdio.h>      // printf, fprintf
#include <stdlib.h>     // exit, atoi, EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>     // fork, execl, setsid, chdir, close, sleep, sysconf, _SC_OPEN_MAX
#include <signal.h>     // signal, SIGCHLD, SIGHUP, SIG_IGN
#include <sys/types.h>  // pid_t
#include <sys/stat.h>   // umask
#include <syslog.h>     // syslog, openlog, LOG_ERR, LOG_INFO, LOG_NOTICE, LOG_PID, LOG_DAEMON
#include <time.h>       // time, localtime, struct tm
#include <string.h>     // strerror
#include <errno.h>      // errno
#include <sys/wait.h>   // waitpid


void run_script(const char* script) {
    pid_t pid = fork();

    if (pid < 0) {
        syslog(LOG_ERR, "Fork failed: %s", strerror(errno));
        return;
    }

    if (pid == 0) {  // Child process
        syslog(LOG_INFO, "Executing script: %s", script);
        execl("/bin/sh", "sh", script, (char *)NULL);
        // If execl returns, it must have failed
        syslog(LOG_ERR, "execl failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    } else {  // Parent process
        // Optionally, wait for child to finish
        waitpid(pid, NULL, 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <script_path> <hour> <minute>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* script_path = argv[1];
    int target_hour = atoi(argv[2]);
    int target_minute = atoi(argv[3]);

    // Validate input
    if (target_hour < 0 || target_hour > 23 || target_minute < 0 || target_minute > 59) {
        fprintf(stderr, "Invalid time. Hour must be 0-23, minute must be 0-59.\n");
        exit(EXIT_FAILURE);
    }

    // Create daemon process
    pid_t pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);  // Parent process exits
    }

    // Child process becomes session leader
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    // Ignore certain signals
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // Fork again
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);  // Parent process exits
    }

    // Change working directory to root to ensure consistent behavior
    chdir("/");

    // Close all inherited file descriptors to ensure a clean environment for executed scripts
    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    // Open syslog
    openlog("time_based_daemon", LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "Daemon started");

    while (1) {
        time_t now = time(NULL);
        struct tm *tm_now = localtime(&now);

        if (tm_now->tm_hour == target_hour && tm_now->tm_min == target_minute) {
            run_script(script_path);
            sleep(60);  // Sleep for a minute to avoid multiple executions
        }

        sleep(30);  // Check every 30 seconds
    }

    closelog();
    return EXIT_SUCCESS;
}
