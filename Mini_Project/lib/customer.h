#ifndef IGNORE_IMPORTS
#include "./schema.h"
#include "./console.h"
#include <fcntl.h>
#include <unistd.h>
#endif

// -1: failed
float customer_view_balance(int *client_socket, char *username) {
    int fd = open("./data/customer.dat", O_RDONLY);
    if (fd == -1) {
        perror("Error opening customer.dat file");
        return -1;
    }
    
    float result = -1;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Customer_S temp;    
    while (read(fd, &temp, sizeof(struct Customer_S)) > 0) {
        if (strcmp(temp.username, username) == 0) {
            result = temp.savings_acc_balance;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    close(fd);
    return result;
}

// -1: invalid username, 0: wrong password, 1: verified
int customer_verify_password(char *username, char *password, struct Customer_S *customer_data) {
    int fd = open("./data/customer.dat", O_RDONLY);
    if (fd == -1) {
        perror("Error opening customer.dat file");
        return -1;
    }
    
    int result = -1;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Customer_S temp;    
    while (read(fd, &temp, sizeof(struct Customer_S)) > 0) {
        if (strcmp(temp.username, username) == 0) {
            if (strcmp(temp.password, password) == 0) {
                strcpy(customer_data->username, temp.username);
                strcpy(customer_data->password, temp.password);
                strcpy(customer_data->fullname, temp.fullname);

                result = 1;
            }
            else
                result = 0;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    close(fd);
    return result;
}


// -2: insufficient balance, -1: failed, 1: balance changed
int customer_change_balance(char *username, float amount) {
    int fd = open("./data/customer.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening customer.dat file");
        return -1;
    }

    // =======================================
    // read => find index
    int result = -1;
    int idx = -1;
    int flag = 0;   // set if we find a match
    

    struct Customer_S temp;
    while (read(fd, &temp, sizeof(struct Customer_S)) > 0) {
        idx++;
        if (strcmp(temp.username, username) == 0) {
            flag = 1;   // match found
            break;
        }
    }

    // =======================================
    // lock => modify
    if(flag == 1) {
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = idx * sizeof(struct Customer_S);
        lock.l_len = sizeof(struct Customer_S);

        fcntl(fd, F_SETLKW, &lock);


        lseek(fd, idx * sizeof(struct Customer_S), SEEK_SET);
        read(fd, &temp, sizeof(struct Customer_S));

        lseek(fd, -1 * sizeof(struct Customer_S), SEEK_CUR);

        if(temp.savings_acc_balance + amount >= 0) {
            temp.savings_acc_balance += amount;
            write(fd, &temp, sizeof(struct Customer_S));
            result = 1;
        } else {
            result = -2;
        }


        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
    }

    return result;
}

//-2: insufficient balance, -1: failed, 0: invalid amount, 1: amount withdrawn
int customer_withdraw_money(int *client_socket, char *username) {
    float amount_to_withdraw = 0;
    read(*client_socket, &amount_to_withdraw, sizeof(amount_to_withdraw));

    if(amount_to_withdraw <= 0) {
        return 0;
    }

    return customer_change_balance(username, (-1*amount_to_withdraw));
}

// -1: failed, 0: invalid amount, 1: amount withdrawn
int customer_deposit_money(int *client_socket, char *username) {
    float amount_to_withdraw = 0;
    read(*client_socket, &amount_to_withdraw, sizeof(amount_to_withdraw));
    if(amount_to_withdraw <= 0) {
        return 0;
    }

    return customer_change_balance(username, amount_to_withdraw);
}


void handle_customer_menu(int *client_socket, struct Customer_S *customer_data) {
    write(*client_socket, CUSTOMER_MENU_MSG, strlen(CUSTOMER_MENU_MSG));

    int operation=1;
    int status = -1;
    while(read(*client_socket, &operation, sizeof(operation)) && operation > 0 && operation < 10 ) {
        switch (operation)
        {
        case 1:
            float savings_acc_balance = customer_view_balance(client_socket, customer_data->username);
            write(*client_socket, &savings_acc_balance, sizeof(savings_acc_balance));
            break;

        case 2:
            status = customer_withdraw_money(client_socket, customer_data->username);
            write(*client_socket, &status, sizeof(status));
            break;

        case 3:
            status = customer_deposit_money(client_socket, customer_data->username);
            write(*client_socket, &status, sizeof(status));
            break;
            
        case 4:
            // status = customer_deposit_money(client_socket, customer_data->username);
            // write(*client_socket, &status, sizeof(status));
            break;

        case 5:
            // status = admin_manage_user_roles(client_socket);
            // write(*client_socket, &status, sizeof(status));
            break;
        case 6:
            // status = admin_change_password(client_socket);
            // write(*client_socket, &status, sizeof(status));
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        }

    }
}

void handle_customer_login(int *client_socket, char *username) {
    write(*client_socket, MAIN_MENU_ASK_PASSWORD, strlen(MAIN_MENU_ASK_PASSWORD));

    char password[PASSWORD_LEN];
    read(*client_socket, &password, sizeof(password));

    struct Customer_S customer_data;
    int customer_verify_password_status = customer_verify_password(username, password, &customer_data);
    write(*client_socket, &customer_verify_password_status, sizeof(customer_verify_password_status));

    if(customer_verify_password_status == 1) {
        handle_customer_menu(client_socket, &customer_data);
    }
}