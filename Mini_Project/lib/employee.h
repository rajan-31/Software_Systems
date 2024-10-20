#ifndef IGNORE_IMPORTS
#include "./schema.h"
#include "./console.h"
#include <fcntl.h>
#include <unistd.h>
#endif

// -1: failed, 0: new pass mismatch, 1: success
int employee_change_password(int *client_socket, char *username) {
    int is_password_confirmed = -1;
    read(*client_socket, &is_password_confirmed, sizeof(is_password_confirmed));

    if(is_password_confirmed != 1)
        return 0;

    
    // =======================================
    char new_password[PASSWORD_LEN];
    read(*client_socket, &new_password, sizeof(new_password));

    int fd = open("./data/employee.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening employee.dat file");
        return -1;
    }

    int result = -1;
    int idx = -1;
    int flag = 0;

    struct Employee_S temp;
    while (read(fd, &temp, sizeof(struct Employee_S)) > 0) {
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
        lock.l_start = idx * sizeof(struct Employee_S);
        lock.l_len = sizeof(struct Employee_S);

        fcntl(fd, F_SETLKW, &lock);


        lseek(fd, idx * sizeof(struct Employee_S), SEEK_SET);
        read(fd, &temp, sizeof(struct Employee_S));

        lseek(fd, -1 * sizeof(struct Employee_S), SEEK_CUR);

        strcpy(temp.password, new_password);
        write(fd, &temp, sizeof(struct Employee_S));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
        result = 1;
    }

    close(fd);

    return result;
}

// -1: invalid username, 0: wrong password, 1: verified
int employee_verify_password(char *username, char *password, struct Employee_S *employee_data) {
    int fd = open("./data/employee.dat", O_RDONLY);
    if (fd == -1) {
        perror("Error opening employee.dat file");
        return -1;
    }
    
    int result = -1;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Employee_S temp;    
    while (read(fd, &temp, sizeof(struct Employee_S)) > 0) {
        if (strcmp(temp.username, username) == 0) {
            if (strcmp(temp.password, password) == 0) {
                strcpy(employee_data->username, temp.username);
                strcpy(employee_data->password, temp.password);
                strcpy(employee_data->fullname, temp.fullname);
                employee_data->role = temp.role;

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

// -1: failed, 0: duplicate username, 1: added
int employee_add_new_customer(int *client_socket) {
    struct Customer_S customer_data;
    read(*client_socket, &customer_data, sizeof(customer_data));
    customer_data.active = 1;
    customer_data.savings_acc_balance = 0;
    strcpy(customer_data.savings_acc_num, "");
    
    int fd = open("./data/customer.dat", O_WRONLY);
    if (fd == -1) {
        perror("Error opening customer.dat file");
        return -1;
    }

    int result = -1;

    lseek(fd, 0, SEEK_END);

    
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = 0;
    lock.l_len = sizeof(struct Customer_S);

    fcntl(fd, F_SETLKW, &lock);

    if (write(fd, &customer_data, sizeof(struct Customer_S)) != sizeof(struct Customer_S)) {
        perror("Error writing to file");
    } else
        result = 1;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    close(fd);
    return result;
}


void employee_view_customer_transaction_history(int *client_socket) {
    char customer_username[USERNAME_LEN];
    read(*client_socket, customer_username, sizeof(customer_username));
    
    int tx_history_capacity = 100;
    struct Transaction_S *tx_history = (struct Transaction_S *) malloc(tx_history_capacity * sizeof(struct Transaction_S));

    int fd = open("./data/transaction.dat", O_RDONLY);

    struct Transaction_S temp;
    int tx_history_size = 0;
    while(read(fd, &temp, sizeof(struct Transaction_S)) > 0) {
        if(tx_history_size >= tx_history_capacity) {
            tx_history_capacity *= 2;
            struct Transaction_S *new_tx_history = realloc(tx_history, tx_history_capacity * sizeof(struct Transaction_S));
            tx_history = new_tx_history;
        }

        int to_include = 0;
        if(strcmp(temp.payee, customer_username) == 0 || strcmp(temp.payer, customer_username) == 0) {
            if(strcmp(temp.payer, temp.payee) == 0) {
                to_include = 1;
            } else if(strcmp(temp.payer, customer_username) == 0) {
                temp.payee_balance = -1;
                to_include = 1;
            } else if(strcmp(temp.payee, customer_username) == 0) {
                to_include = 1;
                temp.payer_balance = -1;
            }
        }

        if(to_include == 1)
            tx_history[tx_history_size++] = temp;
    }

    close(fd);

    write(*client_socket, &tx_history_size, sizeof(tx_history_size));
    if(tx_history_size > 0) {
        write(*client_socket, tx_history, tx_history_size * sizeof(struct Transaction_S));
    }

    free(tx_history);
}

void employee_view_customers(int *client_socket) {
    int customers_list_capacity = 100;
    struct Customer_S *customers_list = (struct Customer_S *) malloc(customers_list_capacity * sizeof(struct Customer_S));

    int fd = open("./data/customer.dat", O_RDONLY);

    struct Customer_S temp;
    int customers_list_size = 0;
    while(read(fd, &temp, sizeof(struct Customer_S)) > 0) {
        if(customers_list_size >= customers_list_capacity) {
            customers_list_capacity *= 2;
            struct Customer_S *new_customers_list = realloc(customers_list, customers_list_capacity * sizeof(struct Customer_S));
            customers_list = new_customers_list;
        }
        
        strcpy(temp.password, "");
        customers_list[customers_list_size++] = temp;
    }

    close(fd);

    write(*client_socket, &customers_list_size, sizeof(customers_list_size));
    if(customers_list_size > 0) {
        write(*client_socket, customers_list, customers_list_size * sizeof(struct Customer_S));
    }

    free(customers_list);
}

void employee_view_assigned_loan_applications(int *client_socket, char *username) {
    int loan_applications_capacity = 100;
    struct Loan_Account_S *loan_applications = (struct Loan_Account_S *) malloc(loan_applications_capacity * sizeof(struct Loan_Account_S));

    int fd = open("./data/loan.dat", O_RDONLY);

    struct Loan_Account_S temp;
    int loan_applications_size = 0;
    while(read(fd, &temp, sizeof(struct Loan_Account_S)) > 0) {
        if(loan_applications_size >= loan_applications_capacity) {
            loan_applications_capacity *= 2;
            struct Loan_Account_S *new_loan_applications = realloc(loan_applications, loan_applications_capacity * sizeof(struct Loan_Account_S));
            loan_applications = new_loan_applications;
        }
        
        if(strcmp(temp.assigned_employee, username) == 0)
            loan_applications[loan_applications_size++] = temp;
    }

    close(fd);

    write(*client_socket, &loan_applications_size, sizeof(loan_applications_size));
    if(loan_applications_size > 0) {
        write(*client_socket, loan_applications, loan_applications_size * sizeof(struct Loan_Account_S));
    }

    free(loan_applications);
}

int employee_process_loan_application(int *client_socket) {
    // client inputs
    char loan_acc_num[ACCOUNT_NUM_LEN];
    read(*client_socket, loan_acc_num, sizeof(loan_acc_num));

    int fd = open("./data/loan.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening loan.dat file");
        return -1;
    }

    // =======================================
    // read => find index
    int result = -1;
    int idx = -1;
    int flag = 0;   // set if we find a match
    

    struct Loan_Account_S temp;
    while (read(fd, &temp, sizeof(struct Loan_Account_S)) > 0) {
        idx++;
        if (strcmp(temp.loan_acc_num, loan_acc_num) == 0) {
            flag = 1;   // match found
            break;
        }
    }


    // =======================================
    // lock => modify
    if(flag == 1) {
        write(*client_socket, &temp, sizeof(struct Loan_Account_S));

        int action = 0;
        read(*client_socket, &action, sizeof(action));

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = idx * sizeof(struct Loan_Account_S);
        lock.l_len = sizeof(struct Loan_Account_S);

        fcntl(fd, F_SETLKW, &lock);


        lseek(fd, idx * sizeof(struct Loan_Account_S), SEEK_SET);
        read(fd, &temp, sizeof(struct Loan_Account_S));

        lseek(fd, -1 * sizeof(struct Loan_Account_S), SEEK_CUR);

        temp.processed = action;
        write(fd, &temp, sizeof(struct Loan_Account_S));
        
        result = 1;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
    }

    close(fd);
    return result;
}

int employee_accept_reject_loan_application(int *client_socket) {
    // client inputs
    char loan_acc_num[ACCOUNT_NUM_LEN];
    read(*client_socket, loan_acc_num, sizeof(loan_acc_num));

    int fd = open("./data/loan.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening loan.dat file");
        return -1;
    }

    // =======================================
    // read => find index
    int result = -1;
    int idx = -1;
    int flag = 0;   // set if we find a match
    

    struct Loan_Account_S temp;
    while (read(fd, &temp, sizeof(struct Loan_Account_S)) > 0) {
        idx++;
        if (strcmp(temp.loan_acc_num, loan_acc_num) == 0) {
            flag = 1;   // match found
            break;
        }
    }


    // =======================================
    // lock => modify
    if(flag == 1) {
        write(*client_socket, &temp, sizeof(struct Loan_Account_S));

        int action = 0;
        read(*client_socket, &action, sizeof(action));

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = idx * sizeof(struct Loan_Account_S);
        lock.l_len = sizeof(struct Loan_Account_S);

        fcntl(fd, F_SETLKW, &lock);


        lseek(fd, idx * sizeof(struct Loan_Account_S), SEEK_SET);
        read(fd, &temp, sizeof(struct Loan_Account_S));

        lseek(fd, -1 * sizeof(struct Loan_Account_S), SEEK_CUR);

        temp.accepted = action;
        write(fd, &temp, sizeof(struct Loan_Account_S));
        
        result = 1;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
    }

    close(fd);
    return result;
}

// =======================================

void handle_employee_menu(int *client_socket, struct Employee_S *employee_data) {
    write(*client_socket, EMPLOYEE_MENU_MSG, strlen(EMPLOYEE_MENU_MSG));

    int operation=1;
    int status = -1;
    while(read(*client_socket, &operation, sizeof(operation)) && operation > 0 && operation < 10 ) {
        switch (operation)
        {
        case 1:
            employee_view_customers(client_socket);
            break;

        case 2:
            employee_view_customer_transaction_history(client_socket);
            break;

        case 3:
            status = employee_add_new_customer(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;
            
        case 4:
            status = admin_modify_customer_details(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;

        case 5:
            employee_view_assigned_loan_applications(client_socket, employee_data->username);
            break;

        case 6:
            status = employee_process_loan_application(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;

        case 7:
            status = employee_accept_reject_loan_application(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;
            
        case 8:
            status = employee_change_password(client_socket, employee_data->username);
            write(*client_socket, &status, sizeof(status));
            break;

        case 9:
            // Logout
            break;
        }

    }
}

void handle_employee_login(int *client_socket, char *username) {
    write(*client_socket, MAIN_MENU_ASK_PASSWORD, strlen(MAIN_MENU_ASK_PASSWORD));

    char password[PASSWORD_LEN];
    read(*client_socket, &password, sizeof(password));

    struct Employee_S employee_data;
    int employee_verify_password_status = employee_verify_password(username, password, &employee_data);
    write(*client_socket, &employee_verify_password_status, sizeof(employee_verify_password_status));

    if(employee_verify_password_status == 1) {
        handle_employee_menu(client_socket, &employee_data);
    }
}