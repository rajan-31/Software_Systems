// -1: fail, 0: session exists, 1: assigned new session, 2: no session available
int do_active_session_assign(struct Session_S *active_sessions, int n, pthread_mutex_t *mutex_active_sessions, char *username, int *client_socket) {
    int result = -1;
    pthread_mutex_lock(mutex_active_sessions);

    for(int i=0; i<=n-1; i++) {
        if(strcmp(active_sessions[i].username, username) == 0) {
            result = 0;
            break;
        }
    }

    if(result != 0) {
        for(int i=0; i<=n; i++) {
            if(active_sessions[i].username[0] == '\0') {
                strcpy(active_sessions[i].username, username);
                active_sessions[i].socket_fd = *client_socket;

                result = 1;
                break;
            }
        }

        if(result != 1)
            result = 2;
    }

    pthread_mutex_unlock(mutex_active_sessions);

    return result;
}

void do_active_session_remove(struct Session_S *active_sessions, int n, pthread_mutex_t *mutex_active_sessions, char *username) {
    pthread_mutex_lock(mutex_active_sessions);

    for(int i=0; i<=n-1; i++) {
        if(strcmp(active_sessions[i].username, username) == 0) {
            strcpy(active_sessions[i].username, "");
            active_sessions[i].socket_fd = 0;
        }
    }

    pthread_mutex_unlock(mutex_active_sessions);
}

void do_active_session_print(struct Session_S *active_sessions, int n, pthread_mutex_t *mutex_active_sessions) {
    pthread_mutex_lock(mutex_active_sessions);

    int count = 0;

    printf("\nPrinting active sessions: \n");
    for(int i=0; i<=n-1; i++) {
        if(active_sessions[i].username[0] != '\0') {
            printf("Username: %s\tSocket_fd: %d\n", active_sessions[i].username, active_sessions[i].socket_fd);
            count++;
        }
    }

    if (count == 0)
        printf("No Active Session!\n");
    

    pthread_mutex_unlock(mutex_active_sessions);
}