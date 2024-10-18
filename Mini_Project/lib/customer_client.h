#ifndef IGNORE_IMPORTS
#include "./schema.h"
#include <stdio.h>
#include <stdlib.h>
#endif

void customer_view_balance(int *client_fd) {
    float savings_acc_balance = -1;
    read(*client_fd, &savings_acc_balance, sizeof(savings_acc_balance));

    if(savings_acc_balance < 0) {
        printf("Failed to View Balance!\n");
    } else {
        printf("Savings Account Balance: %.2f\n", savings_acc_balance);
    }
}

void customer_withdraw_money(int *client_fd) {
    float amount_to_withdraw = 0;
    printf("Enter Amount to Withdraw: "); scanf("%f", &amount_to_withdraw);
    write(*client_fd, &amount_to_withdraw, sizeof(amount_to_withdraw));

    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -2) {
        printf("Insufficient Balance!\n");
    } else if(status == -1) {
        printf("Failed to Withdraw Money!\n");
    } else if(status == 0) {
        printf("Invalid Amount Entered!\n");
    } else if(status == 1) {
        printf("Money Withdrawn Successfully\n");
    }
}

void customer_deposit_money(int *client_fd) {
    float amount_to_deposit = 0;
    printf("Enter Amount to Deposit: "); scanf("%f", &amount_to_deposit);
    write(*client_fd, &amount_to_deposit, sizeof(amount_to_deposit));

    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to Deposit Money!\n");
    } else if(status == 0) {
        printf("Invalid Amount Entered!\n");
    } else if(status == 1) {
        printf("Money Deposited Successfully\n");
    }
}

int handle_customer_menu(int *client_fd, char *username, char *password){
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
            customer_view_balance(client_fd);
            break;

        case 2:
            customer_withdraw_money(client_fd);
            break;

        case 3:
            customer_deposit_money(client_fd);
            break;
        case 4:
            // admin_manage_user_roles(client_fd);
            break;

        case 5:
            // admin_change_password(client_fd, username);
            break;
        case 6:
            // admin_change_password(client_fd, username);
            break;
        case 7:
            // admin_change_password(client_fd, username);
            break;
        case 8:
            // admin_change_password(client_fd, username);
            break;
        case 9:
            return 1;
        }
    } while(operation > 0 && operation < 10 );

    return -1;
}