#include <stdio.h>
#include <sys/socket.h>     // socket, AF_INET, SOCK_STREAM, INADDR_ANY,  SOL_SOCKET, SO_REUSEADDR, SO_KEEPALIVE
#include <arpa/inet.h>  // sockaddr_in
#include <pthread.h>    // pthread_t, pthread_create
#include <string.h>
#include <stdlib.h> // free
#include <unistd.h> // read, write

#define IGNORE_IMPORTS

#include "./lib/schema.h"
#include "./lib/admin_client.h"
#include "./lib/customer_client.h"
#include "./lib/employee_client.h"
#include "./lib/manager_client.h"

int main() {
    goto_connect:
    int do_goto_connect = 0;
    
    unsigned int PORT = 8080;
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET; serv_addr.sin_port = htons(PORT);

    // accept only from (converted string to bin for ipv4)
    int convert_status = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);



    int connect_status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char buffer1[1000] = {0};
    if(read(client_fd, &buffer1, sizeof(buffer1)) <= 0) {
        close(client_fd);
        return 0;
    }
    printf("%s", buffer1);

    int user_role = 3;
    // scanf("%d", &user_role);
    write(client_fd, &user_role, sizeof(user_role));

    if(user_role < 0 || user_role > 4) {
        close(client_fd);
        return 0;
    } else {
        char buffer2[100] = {0};
        read(client_fd, &buffer2, sizeof(buffer2));
        printf("%s", buffer2);

        char username[USERNAME_LEN] = "emp4";
        // scanf("%s", username);
        write(client_fd, &username, sizeof(username));
        
        int active_session_assign = -2;
        read(client_fd, &active_session_assign, sizeof(active_session_assign));

        if(active_session_assign == 1) {
            char buffer3[100] = {0};
            read(client_fd, &buffer3, sizeof(buffer3));
            printf("%s", buffer3);

            char password[PASSWORD_LEN] = "123456";
            // scanf("%s", password);
            write(client_fd, &password, sizeof(password));


            int verify_user_password_status;
            read(client_fd, &verify_user_password_status, sizeof(verify_user_password_status));

            // -1: invalid username, 0: wrong password, 1: verified
            if(verify_user_password_status == -1) {
                printf("Invalid Username!\n");
            } else if(verify_user_password_status == 0) {
                printf("Invalid Password!\n");
            } else if(verify_user_password_status == 1) {
                if(user_role == 1) {
                    do_goto_connect = handle_admin_menu(&client_fd, username, password);
                } else if(user_role == 2) {
                    do_goto_connect = handle_manager_menu(&client_fd, username, password);
                } else if(user_role == 3) {
                    do_goto_connect = handle_employee_menu(&client_fd, username, password);
                } else if(user_role == 4) {
                    do_goto_connect = handle_customer_menu(&client_fd, username, password);
                }
            }

        } else if(active_session_assign == 0) {
            printf("\nOnly one session per user is allowed!\n");
        } else if(active_session_assign == 2) {
            printf("\nNo session availabele, plaese try later!\n");
        }
    }

    close(client_fd);

    if(do_goto_connect == 1)
        goto goto_connect;

    return 0;
}