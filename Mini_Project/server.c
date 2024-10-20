#include <stdio.h>
#include <sys/socket.h>     // socket, AF_INET, SOCK_STREAM, INADDR_ANY,  SOL_SOCKET, SO_REUSEADDR, SO_KEEPALIVE
#include <arpa/inet.h>  // sockaddr_in
#include <pthread.h>    // pthread_t, pthread_create
#include <string.h>
#include <stdlib.h> //free
#include <unistd.h> // read, write
#include <fcntl.h>

#define IGNORE_IMPORTS

#include "./lib/schema.h"
#include "./lib/console.h"
#include "./lib/common.h"
#include "./lib/server.h"
#include "./lib/admin.h"
#include "./lib/customer.h"
#include "./lib/employee.h"
#include "./lib/manager.h"

const int PORT = 8080;
const int REQUESTS_QUEUE_SIZE = 100;
#define MAX_ACTIVE_SESSIONS 100


struct Session_S active_sessions[MAX_ACTIVE_SESSIONS];
pthread_mutex_t mutex_active_sessions = PTHREAD_MUTEX_INITIALIZER;


void *handle_client(void *arg) {
    int client_socket = *(int*)arg;

    write(client_socket, MAIN_MENU_MSG, strlen(MAIN_MENU_MSG));

    enum role_type user_role;
    read(client_socket, &user_role, sizeof(user_role));
    
    if(user_role > 0 && user_role < 5) {
        write(client_socket, MAIN_MENU_ASK_USERNAME, strlen(MAIN_MENU_ASK_USERNAME));

        char username[USERNAME_LEN];
        int temp = read(client_socket, &username, sizeof(username));

        int active_session_assign = do_active_session_assign(active_sessions, MAX_ACTIVE_SESSIONS, &mutex_active_sessions, username, &client_socket);
        write(client_socket, &active_session_assign, sizeof(active_session_assign));

        do_active_session_print(active_sessions, MAX_ACTIVE_SESSIONS, &mutex_active_sessions);

        
        if(active_session_assign == 1) {
            if(user_role == ADMIN_E) {
                handle_admin_login(&client_socket, username);
            } else if(user_role == MANAGER_E) {
                handle_manager_login(&client_socket, username);
            } else if(user_role == EMPLOYEE_E) {
                handle_employee_login(&client_socket, username);
            } else if(user_role == CUSTOMER_E) {
                handle_customer_login(&client_socket, username);
            }

            do_active_session_remove(active_sessions, MAX_ACTIVE_SESSIONS, &mutex_active_sessions, username);
        }

        do_active_session_print(active_sessions, MAX_ACTIVE_SESSIONS, &mutex_active_sessions);
    }

    close(client_socket);
    free(arg);
    return NULL;
}

int main() {
    // IPv4, TCP, default
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // reuse socket, keep conn alive
    int opt = 1;
    int sockopt_status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_KEEPALIVE, &opt, sizeof(opt));

    // any incoming addr
    struct sockaddr_in address; socklen_t addrlen = sizeof(address);
    address.sin_family = AF_INET; address.sin_addr.s_addr = INADDR_ANY; address.sin_port = htons(PORT);

    int bind_status = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    int listen_status = listen(server_fd, REQUESTS_QUEUE_SIZE);

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        int *client_socket = malloc(sizeof(int));
        *client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*)client_socket);
        pthread_detach(thread_id);
    }

    return 0;
}