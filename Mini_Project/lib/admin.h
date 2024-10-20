#ifndef IGNORE_IMPORTS
#include "./schema.h"
#include "./console.h"
#include "./server.h"
#endif

// -1: invalid username, 0: wrong password, 1: verified
int admin_verify_password(char *username, char *password, struct Admin_S *admin_data) {
    int fd = open("./data/admin.dat", O_RDONLY);
    if (fd == -1) {
        perror("Error opening admin.dat file");
        return -1;
    }
    
    int result = -1;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Admin_S temp;    
    while (read(fd, &temp, sizeof(struct Admin_S)) > 0) {
        if (strcmp(temp.username, username) == 0) {
            if (strcmp(temp.password, password) == 0) {
                strcpy(admin_data->username, temp.username);
                strcpy(admin_data->password, temp.password);
                strcpy(admin_data->fullname, temp.fullname);

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
int admin_add_new_bank_employee(int *client_socket) {
    struct Employee_S employee_data;
    read(*client_socket, &employee_data, sizeof(employee_data));
    employee_data.role = EMPLOYEE_E;
    
    int fd = open("./data/employee.dat", O_WRONLY);
    if (fd == -1) {
        perror("Error opening employee.dat file");
        return -1;
    }

    int result = -1;

    lseek(fd, 0, SEEK_END);

    
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = 0;
    lock.l_len = sizeof(employee_data);

    fcntl(fd, F_SETLKW, &lock);

    if (write(fd, &employee_data, sizeof(struct Employee_S)) != sizeof(struct Employee_S)) {
        perror("Error writing to file");
    } else
        result = 1;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    close(fd);
    return result;
}

// -1: failed, 0: invalid username, 1: modified
int admin_modify_employee_details(int *client_socket) {
    // client inputs
    struct Employee_S employee_data;
    read(*client_socket, &employee_data, sizeof(employee_data));

    int fd = open("./data/employee.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening employee.dat file");
        return -1;
    }

    // =======================================
    // lock => read => find index
    int result = -1;
    int idx = -1;
    int flag = 0;   // set if we find a match

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Employee_S temp;
    while (read(fd, &temp, sizeof(struct Employee_S)) > 0) {
        idx++;
        if (strcmp(temp.username, employee_data.username) == 0) {
            // useful while modifying, since employee_data don't have role
            employee_data.role = temp.role;

            flag = 1;   // match found
            break;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    // =======================================
    // lock => modify
    if(flag == 1) {
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = idx * sizeof(struct Employee_S);
        lock.l_len = sizeof(struct Employee_S);

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, idx * sizeof(struct Employee_S), SEEK_SET);

        write(fd, &employee_data, sizeof(struct Employee_S));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);

        result = 1;
    } else {
        result = 0;
    }

    return result;
}

// -1: failed, 0: invalid username, 1: modified
int admin_modify_customer_details(int *client_socket) {
    // client inputs
    struct Customer_S customer_data;
    read(*client_socket, &customer_data, sizeof(struct Customer_S));

    int fd = open("./data/customer.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening customer.dat file");
        return -1;
    }

    // =======================================
    // lock => read => find index
    int result = -1;
    int idx = -1;
    int flag = 0;   // set if we find a match

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Customer_S temp;
    while (read(fd, &temp, sizeof(struct Customer_S)) > 0) {
        idx++;
        if (strcmp(temp.username, customer_data.username) == 0) {
            // useful while modifying, since customer_data don't have role
            customer_data.active = temp.active;
            strcpy(customer_data.password, temp.password);
            strcpy(customer_data.savings_acc_num, temp.savings_acc_num);
            customer_data.savings_acc_balance = temp.savings_acc_balance;

            flag = 1;   // match found
            break;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    // =======================================
    // lock => modify
    if(flag == 1) {
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = idx * sizeof(struct Customer_S);
        lock.l_len = sizeof(struct Customer_S);

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, idx * sizeof(struct Customer_S), SEEK_SET);

        write(fd, &customer_data, sizeof(struct Customer_S));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);

        result = 1;
    } else {
        result = 0;
    }

    return result;
}

// -1: failed, 0: invalid username, 1: modified
int admin_manage_user_roles(int *client_socket) {
    // client inputs
    struct Employee_S employee_data;
    read(*client_socket, &employee_data, sizeof(employee_data));

    int fd = open("./data/employee.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening employee.dat file");
        return -1;
    }

    // =======================================
    // lock => read => find index
    int result = -1;
    int idx = -1;
    int flag = 0;   // set if we find a match

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Employee_S temp;
    while (read(fd, &temp, sizeof(struct Employee_S)) > 0) {
        idx++;
        if (strcmp(temp.username, employee_data.username) == 0) {
            // useful while modifying, since employee_data don't have role
            strcpy(employee_data.password, temp.password);
            strcpy(employee_data.fullname, temp.fullname);

            flag = 1;   // match found
            break;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    // =======================================
    // lock => modify
    if(flag == 1) {
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = idx * sizeof(struct Employee_S);
        lock.l_len = sizeof(struct Employee_S);

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, idx * sizeof(struct Employee_S), SEEK_SET);

        write(fd, &employee_data, sizeof(struct Employee_S));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);

        result = 1;
    } else {
        result = 0;
    }

    return result;
}

// -1: failed, 0: new password missmatch, 1: modified
int admin_change_password(int *client_socket) {
    int is_password_confirmed = -1;
    read(*client_socket, &is_password_confirmed, sizeof(is_password_confirmed));

    if(is_password_confirmed != 1)
        return 0;

    
    // =======================================
    struct Admin_S admin_data;
    read(*client_socket, &admin_data, sizeof(admin_data));

    int fd = open("./data/admin.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening admin.dat file");
        return -1;
    }

    // =======================================
    // lock => read => find index
    int result = -1;
    int idx = -1;
    int flag = 0;   // set if we find a match

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Admin_S temp;
    while (read(fd, &temp, sizeof(struct Admin_S)) > 0) {
        idx++;
        if (strcmp(temp.username, admin_data.username) == 0) {
            // useful while modifying, since admin_data don't have role
            strcpy(admin_data.fullname, temp.fullname);

            flag = 1;   // match found
            break;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    // =======================================
    // lock => modify
    if(flag == 1) {
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = idx * sizeof(struct Admin_S);
        lock.l_len = sizeof(struct Admin_S);

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, idx * sizeof(struct Admin_S), SEEK_SET);

        write(fd, &admin_data, sizeof(struct Admin_S));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);

        result = 1;
    } else {
        result = -1;
    }

    return result;

}

void handle_admin_menu(int *client_socket, struct Admin_S *admin_data) {
    write(*client_socket, ADMIN_MENU_MSG, strlen(ADMIN_MENU_MSG));

    int operation=1;
    int status = -1;
    while(read(*client_socket, &operation, sizeof(operation)) && operation > 0 && operation < 7 ) {
        switch (operation)
        {
        case 1:
            status = admin_add_new_bank_employee(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;

        case 2:
            status = admin_modify_employee_details(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;
            
        case 3:
            status = admin_modify_customer_details(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;

        case 4:
            status = admin_manage_user_roles(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;
        case 5:
            status = admin_change_password(client_socket);
            write(*client_socket, &status, sizeof(status));
            break;
        case 6:
            break;
        }

    }
}

void handle_admin_login(int *client_socket, char *username) {
    write(*client_socket, MAIN_MENU_ASK_PASSWORD, strlen(MAIN_MENU_ASK_PASSWORD));

    char password[PASSWORD_LEN];
    read(*client_socket, &password, sizeof(password));

    struct Admin_S admin_data;
    int admin_verify_password_status = admin_verify_password(username, password, &admin_data);
    write(*client_socket, &admin_verify_password_status, sizeof(admin_verify_password_status));

    if(admin_verify_password_status == 1) {
        handle_admin_menu(client_socket, &admin_data);
    }
}
