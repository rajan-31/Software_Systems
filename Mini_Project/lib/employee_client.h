#ifndef IGNORE_IMPORTS
#include "./schema.h"
#include <stdio.h>
#include <stdlib.h>
#endif


void employee_change_password(int *client_fd) {
    char password_1[PASSWORD_LEN];
    char password_2[PASSWORD_LEN];
    printf("Enter New Password: "); scanf("%s", password_1);
    printf("Confirm New Password: "); scanf("%s", password_2);

    int is_password_confirmed = -1;
    if(strcmp(password_1, password_2) == 0) {
        is_password_confirmed = 1;
        write(*client_fd, &is_password_confirmed, sizeof(is_password_confirmed));

        write(*client_fd, &password_1, sizeof(password_1));
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

void employee_add_new_customer(int *client_fd) {
    struct Customer_S customer_data;

    printf("New Customer Username: "); scanf("%s", customer_data.username);
    printf("New Customer Password: "); scanf("%s", customer_data.password);
    printf("New Customer Fullname: "); scanf(" %[^\n]", customer_data.fullname);

    write(*client_fd, &customer_data, sizeof(struct Customer_S));

    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to Add New Customer!\n");
    } else if(status == 0) {
        printf("Can't Add Duplicate Customer!\n");
    } else if(status == 1) {
        printf("Succefully Added New Customer\n");
    }
}

void employee_view_customer_transaction_history(int *client_fd) {
    char customer_username[USERNAME_LEN];
    printf("\nEnter Customer Username: "); scanf("%s", customer_username);
    write(*client_fd, customer_username, sizeof(customer_username));

    int tx_history_size = 0;
    read(*client_fd, &tx_history_size, sizeof(tx_history_size));

    if(tx_history_size == 0) {
        printf("No Transaction History!\n");
        return;
    }

    struct Transaction_S *tx_history = (struct Transaction_S *) malloc(tx_history_size * sizeof(struct Transaction_S));
    read(*client_fd, tx_history, tx_history_size * sizeof(struct Transaction_S));


    printf("\n%-37s %-10s %-8s %-20s %-17s %-17s %-17s\n",
        "Transaction ID", "Date", "Time", "Remarks", "Debit (₹)", "Credit (₹)", "Balance (₹)"
    );

    // Print each transaction in the history
    for (int i = 0; i < tx_history_size; i++) {
        char time_buffer[20];
        struct tm *time_info = localtime(&tx_history[i].timestamp);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);
    
        if(tx_history[i].t_type == CREDIT_E) {
            printf("%-37s %-19s %-20s %-17s %-17.2f %-17.2f\n",
                tx_history[i].transaction_id,
                time_buffer,
                (strcmp(tx_history[i].payer, tx_history[i].payee) == 0) ? "" : tx_history[i].payee,
                "",
                tx_history[i].amount,
                tx_history[i].payer_balance
            );
        } else {
            printf("%-37s %-19s %-20s %-17.2f %17s %-17.2f\n",
                tx_history[i].transaction_id,
                time_buffer,
                (strcmp(tx_history[i].payer, tx_history[i].payee) == 0) ? "" : tx_history[i].payee,
                tx_history[i].amount,
                "",
                tx_history[i].payer_balance
            );
        }
    }
}

void employee_view_customers(int *client_fd) {
    int customers_list_size = 0;
    read(*client_fd, &customers_list_size, sizeof(customers_list_size));

    if(customers_list_size == 0) {
        printf("No Customers!\n");
        return;
    }

    struct Customer_S *customers_list = (struct Customer_S *) malloc(customers_list_size * sizeof(struct Customer_S));
    read(*client_fd, customers_list, customers_list_size * sizeof(struct Customer_S));

    printf("\n%-20s %-20s %-8s %-17s\n",
        "Username", "Fullname", "Status", "Balance"
    );

    for(int i=0;i<=customers_list_size-1; i++) {
        printf("\n%-20s %-20s %-8s %-17.2f\n",
            customers_list[i].username, 
            customers_list[i].fullname,
            customers_list[i].active == 1 ? "Active" : "Inactive",
            customers_list[i].savings_acc_balance
        );
    }
}

void employee_view_assigned_loan_applications(int *client_fd) {
    int loan_applications_size = 0;
    read(*client_fd, &loan_applications_size, sizeof(loan_applications_size));

    if(loan_applications_size == 0) {
        printf("No Loan Applications Assigned to You!\n");
        return;
    }

    struct Loan_Account_S *loan_applications = (struct Loan_Account_S *) malloc(loan_applications_size * sizeof(struct Loan_Account_S));
    read(*client_fd, loan_applications, loan_applications_size * sizeof(struct Loan_Account_S));

    printf("%-15s %-37s %-14s %-12s %-10s %-10s %-15s %-10s %-10s %-10s %-10s\n",
        "username", "acc", "type", "amount", "duration", "annual_inc", "employee", "credit_sc", "interest", "processed", "accepted"
    );

    for(int i=0;i<=loan_applications_size-1; i++) {
        printf("%-15s %-37s %-14s %-12.2f %-10d %-10d %-15s %-10d %-10d %-10d %-10d\n",
            loan_applications[i].username,
            loan_applications[i].loan_acc_num,
            loan_applications[i].type == HOME_LOAN_E ? "Home Loan" :
            loan_applications[i].type == PERSONAL_LOAN_E ? "Personal Loan" :
            loan_applications[i].type == EDUCATION_LOAN_E ? "Education Loan" :
            loan_applications[i].type == VEHICLE_LOAN_E ? "Vehicle Loan" :
            loan_applications[i].type == HOME_LOAN_E ? "Home Loan" :
            loan_applications[i].type == OTHER_LOAN_E ? "Other Loan" : "Unknown",
            loan_applications[i].loan_amount,
            loan_applications[i].loan_repayment_duration_months,
            loan_applications[i].annual_income,
            loan_applications[i].assigned_employee,
            loan_applications[i].credit_score,
            loan_applications[i].interest_rate,
            loan_applications[i].processed,
            loan_applications[i].accepted
        );
    }
}

void employee_process_loan_application(int *client_fd) {
    char loan_acc_num[ACCOUNT_NUM_LEN];
    printf("\nEnter Loan Account Number: "); scanf("%s", loan_acc_num);
    write(*client_fd, loan_acc_num, sizeof(loan_acc_num));

    struct Loan_Account_S loan_account;
    read(*client_fd, &loan_account, sizeof(struct Loan_Account_S));

    printf("%-15s %-37s %-14s %-12s %-10s %-10s %-15s %-10s %-10s %-10s %-10s\n",
        "username", "acc", "type", "amount", "duration", "annual_inc", "employee", "credit_sc", "interest", "processed", "accepted"
    );
    printf("%-15s %-37s %-14s %-12.2f %-10d %-10d %-15s %-10d %-10d %-10d %-10d\n",
        loan_account.username,
        loan_account.loan_acc_num,
        loan_account.type == HOME_LOAN_E ? "Home Loan" :
        loan_account.type == PERSONAL_LOAN_E ? "Personal Loan" :
        loan_account.type == EDUCATION_LOAN_E ? "Education Loan" :
        loan_account.type == VEHICLE_LOAN_E ? "Vehicle Loan" :
        loan_account.type == HOME_LOAN_E ? "Home Loan" :
        loan_account.type == OTHER_LOAN_E ? "Other Loan" : "Unknown",
        loan_account.loan_amount,
        loan_account.loan_repayment_duration_months,
        loan_account.annual_income,
        loan_account.assigned_employee,
        loan_account.credit_score,
        loan_account.interest_rate,
        loan_account.processed,
        loan_account.accepted
    );

    int action = 0;
    printf("\n\n1.Don't Process 2. Process\nAction: ");
    scanf("%d", &action); action -= 1;
    write(*client_fd, &action, sizeof(action));


    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to Perform Action!\n");
    } else if(status == 0) {
        printf("Invalid Loan Account Number!\n");
    } else if(status == 1) {
        printf("Action Successfull\n");
    }
}

void employee_accept_reject_loan_application(int *client_fd) {
    char loan_acc_num[ACCOUNT_NUM_LEN];
    printf("\nEnter Loan Account Number: "); scanf("%s", loan_acc_num);
    write(*client_fd, loan_acc_num, sizeof(loan_acc_num));

    struct Loan_Account_S loan_account;
    read(*client_fd, &loan_account, sizeof(struct Loan_Account_S));

    printf("%-15s %-37s %-14s %-12s %-10s %-10s %-15s %-10s %-10s %-10s %-10s\n",
        "username", "acc", "type", "amount", "duration", "annual_inc", "employee", "credit_sc", "interest", "processed", "accepted"
    );
    printf("%-15s %-37s %-14s %-12.2f %-10d %-10d %-15s %-10d %-10d %-10d %-10d\n",
        loan_account.username,
        loan_account.loan_acc_num,
        loan_account.type == HOME_LOAN_E ? "Home Loan" :
        loan_account.type == PERSONAL_LOAN_E ? "Personal Loan" :
        loan_account.type == EDUCATION_LOAN_E ? "Education Loan" :
        loan_account.type == VEHICLE_LOAN_E ? "Vehicle Loan" :
        loan_account.type == HOME_LOAN_E ? "Home Loan" :
        loan_account.type == OTHER_LOAN_E ? "Other Loan" : "Unknown",
        loan_account.loan_amount,
        loan_account.loan_repayment_duration_months,
        loan_account.annual_income,
        loan_account.assigned_employee,
        loan_account.credit_score,
        loan_account.interest_rate,
        loan_account.processed,
        loan_account.accepted
    );

    int action = 0;
    printf("\n\n1. Reject 2. Accept\nAction: ");
    scanf("%d", &action); action -= 1;
    write(*client_fd, &action, sizeof(action));


    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to Perform Action!\n");
    } else if(status == 0) {
        printf("Invalid Loan Account Number!\n");
    } else if(status == 1) {
        printf("Action Successfull\n");
    }
}

int handle_employee_menu(int *client_fd, char *username, char *password){
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
            employee_view_customers(client_fd);
            break;

        case 2:
            employee_view_customer_transaction_history(client_fd);
            break;

        case 3:
            employee_add_new_customer(client_fd);
            break;

        case 4:
            admin_modify_customer_details(client_fd);
            break;

        case 5:
            employee_view_assigned_loan_applications(client_fd);
            break;

        case 6:
            employee_process_loan_application(client_fd);
            break;

        case 7:
            employee_accept_reject_loan_application(client_fd);
            break;

        case 8:
            employee_change_password(client_fd);
            break;

        case 9:
            // Logout
            return 1;
        }

    } while(operation > 0 && operation < 10 );

    return -1;
}