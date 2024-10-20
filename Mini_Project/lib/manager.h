#ifndef IGNORE_IMPORTS
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "./schema.h"
#include "./console.h"
#include "./employee.h"
#endif

// -1: failed, 0: invalid cust username, 1: success
int manager_activate_deactivate_customer(int *client_socket) {
    char customer_username[USERNAME_LEN];
    read(*client_socket, &customer_username, sizeof(customer_username));

    int do_activate = 1;
    read(*client_socket, &do_activate, sizeof(do_activate));

    int fd = open("./data/customer.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening customer.dat file");
        return -1;
    }

    int result = -1;
    int idx = -1;
    int flag = 0;

    struct Customer_S temp;
    while (read(fd, &temp, sizeof(struct Customer_S)) > 0) {
        idx++;
        if (strcmp(temp.username, customer_username) == 0) {
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

        temp.active = do_activate == 0 ? 0 : 1;
        write(fd, &temp, sizeof(struct Customer_S));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
        result = 1;
    } else {
        result = 0;
    }

    close(fd);

    return result;
}

// -1: failed, 0: invalid employee username, 1: success
int manager_assign_loan_application_process(int *client_socket) {
    char loan_acc_num[ACCOUNT_NUM_LEN];
    read(*client_socket, &loan_acc_num, sizeof(loan_acc_num));

    char employee_username[USERNAME_LEN];
    read(*client_socket, &employee_username, sizeof(employee_username));

    int fd = open("./data/loan.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening loan.dat file");
        return -1;
    }

    int result = -1;
    int idx = -1;
    int flag = 0;

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
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = idx * sizeof(struct Loan_Account_S);
        lock.l_len = sizeof(struct Loan_Account_S);

        fcntl(fd, F_SETLKW, &lock);


        lseek(fd, idx * sizeof(struct Loan_Account_S), SEEK_SET);
        read(fd, &temp, sizeof(struct Loan_Account_S));

        lseek(fd, -1 * sizeof(struct Loan_Account_S), SEEK_CUR);

        strcpy(temp.assigned_employee, employee_username);
        write(fd, &temp, sizeof(struct Loan_Account_S));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
        result = 1;
    } else {
        result = 0;
    }

    close(fd);

    return result;
}

void manager_review_customer_feedbacks(int *client_socket) {
    int feedback_capacity = 100;
    struct Feedback_S *feedback = (struct Feedback_S *) malloc(feedback_capacity * sizeof(struct Feedback_S));

    int fd = open("./data/feedback.dat", O_RDONLY);

    struct Feedback_S temp;
    int feedback_size = 0;
    while(read(fd, &temp, sizeof(struct Feedback_S)) > 0) {
        if(feedback_size >= feedback_capacity) {
            feedback_capacity *= 2;
            struct Feedback_S *new_feedback = realloc(feedback, feedback_capacity * sizeof(struct Feedback_S));
            feedback = new_feedback;
        }

        if(temp.seen == 0)
            feedback[feedback_size++] = temp;
    }

    close(fd);

    printf("Feed size: %d\n", feedback_size);

    write(*client_socket, &feedback_size, sizeof(feedback_size));
    if(feedback_size > 0) {
        write(*client_socket, feedback, feedback_size * sizeof(struct Feedback_S));
    }

    free(feedback);
}

// =======================================

void handle_manager_menu(int *client_socket, struct Employee_S *manager_data) {
    write(*client_socket, MANAGER_MENU_MSG, strlen(MANAGER_MENU_MSG));

    int operation=1;
    int status = -1;
    while(read(*client_socket, &operation, sizeof(operation)) && operation > 0 && operation < 6 ) {
        switch (operation)
        {
        case 1:
            status = manager_activate_deactivate_customer(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;

        case 2:
            status = manager_assign_loan_application_process(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;

        case 3:
            manager_review_customer_feedbacks(client_socket);
            break;
            
        case 4:
            status = employee_change_password(client_socket, manager_data->username);
            write(*client_socket, &status, sizeof(status));
            break;

        case 5:
            // Logout
            break;
        }

    }
}

void handle_manager_login(int *client_socket, char *username) {
    write(*client_socket, MAIN_MENU_ASK_PASSWORD, strlen(MAIN_MENU_ASK_PASSWORD));

    char password[PASSWORD_LEN];
    read(*client_socket, &password, sizeof(password));

    struct Employee_S manager_data;
    int employee_verify_password_status = employee_verify_password(username, password, &manager_data);
    write(*client_socket, &employee_verify_password_status, sizeof(employee_verify_password_status));

    if(employee_verify_password_status == 1) {
        handle_manager_menu(client_socket, &manager_data);
    }
}
