#ifndef IGNORE_IMPORTS
#include "./schema.h"
#include <stdio.h>
#include <stdlib.h>
#endif

void admin_add_new_bank_employee(int *client_fd) {
    struct Employee_S employee_data;

    printf("New Employee Username: "); scanf("%s", employee_data.username);
    printf("New Employee Password: "); scanf("%s", employee_data.password);
    printf("New Employee Fullname: "); scanf(" %[^\n]", employee_data.fullname);

    write(*client_fd, &employee_data, sizeof(struct Employee_S));

    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to add new bank employee!\n");
    } else if(status == 0) {
        printf("Can't add duplicate bank employee!\n");
    } else if(status == 1) {
        printf("Succefully added new bank employee\n");
    }
}

void admin_modify_employee_details(int *client_fd) {
    struct Employee_S employee_data;

    printf("Username of Bank Employee: "); scanf("%s", employee_data.username);
    printf("New Password: "); scanf("%s", employee_data.password);
    printf("New Fullname: "); scanf(" %[^\n]", employee_data.fullname);

    write(*client_fd, &employee_data, sizeof(employee_data));

    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to modify bank employee details!\n");
    } else if(status == 0) {
        printf("Invalid Username!\n");
    } else if(status == 1) {
        printf("Succefully modified bank employee details\n");
    }
}

void admin_modify_customer_details(int *client_fd) {
    struct Customer_S customer_data;

    printf("Username of Customer: "); scanf("%s", customer_data.username);
    // printf("New Password: "); scanf("%s", customer_data.password);
    printf("New Fullname: "); scanf(" %[^\n]", customer_data.fullname);

    write(*client_fd, &customer_data, sizeof(customer_data));

    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to Modify Customer Details!\n");
    } else if(status == 0) {
        printf("Invalid Username!\n");
    } else if(status == 1) {
        printf("Succefully Modified Customer Details\n");
    }
}

void admin_manage_user_roles(int *client_fd) {
    struct Employee_S employee_data;

    printf("Username of Bank Employee: "); scanf("%s", employee_data.username);
    
    int operation = -1;
    printf("Action: 1.Promote\t2.Demote\t3.Skip\n"); scanf("%d", &operation);

    if(operation == 1) {
        employee_data.role = MANAGER_E;
    } else if(operation == 2) {
        employee_data.role = EMPLOYEE_E;
    } else {
        return;
    }

    write(*client_fd, &employee_data, sizeof(employee_data));

    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to modify bank employee details!\n");
    } else if(status == 0) {
        printf("Invalid Username!\n");
    } else if(status == 1) {
        printf("Succefully modified bank employee details\n");
    }
}

void admin_change_password(int *client_fd, char *username) {
    char password_1[PASSWORD_LEN];
    char password_2[PASSWORD_LEN];
    printf("Enter New Password: "); scanf("%s", password_1);
    printf("Confirm New Password: "); scanf("%s", password_2);

    int is_password_confirmed = -1;
    if(strcmp(password_1, password_2) == 0) {
        is_password_confirmed = 1;
        write(*client_fd, &is_password_confirmed, sizeof(is_password_confirmed));

        struct Admin_S admin_data;
        strcpy(admin_data.username, username);
        strcpy(admin_data.password, password_1);

        write(*client_fd, &admin_data, sizeof(struct Admin_S));
    } else {
        is_password_confirmed = 0;
        write(*client_fd, &is_password_confirmed, sizeof(is_password_confirmed));
    }


    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to Change Password!\n");
    } else if(status == 0) {
        printf("New Password Missmatched!\n");
    } else if(status == 1) {
        printf("Password Changed\n");
    }
}

int handle_admin_menu(int *client_fd, char *username, char *password){
    char buffer1[1000] = {0};
    read(*client_fd, &buffer1, sizeof(buffer1));
    
    int operation;
    do {
        printf("%s", buffer1);
        scanf("%d", &operation);
        write(*client_fd, &operation, sizeof(operation));

        switch (operation)
        {
        case 1:
            admin_add_new_bank_employee(client_fd);
            break;

        case 2:
            admin_modify_employee_details(client_fd);
            break;

        case 3:
            admin_modify_customer_details(client_fd);
            break;
        case 4:
            admin_manage_user_roles(client_fd);
            break;

        case 5:
            admin_change_password(client_fd, username);
            break;
        case 6:
            return 1;
        }
    } while(operation > 0 && operation < 7 );

    return -1;
}