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

void customer_transfer_funds(int *client_fd) {
    float amount_to_transfer = 0;
    char receiver[USERNAME_LEN];

    printf("Transfer Funds to: "); scanf("%s", receiver);
    write(*client_fd, &receiver, sizeof(receiver));

    printf("Enter Amount: "); scanf("%f", &amount_to_transfer);
    write(*client_fd, &amount_to_transfer, sizeof(amount_to_transfer));

    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -2) {
        printf("Insufficient Balance!\n");
    } else if(status == -1) {
        printf("Failed to Transfer Funds!\n");
    } else if(status == 0) {
        printf("Invalid Amount Entered!\n");
    } else if(status == 1) {
        printf("Funds Transferred Successfully\n");
    }
}

void customer_view_transaction_history(int *client_fd) {
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

void customer_apply_for_loan(int *client_fd) {
    struct Loan_Account_S loan_account;

    strcpy(loan_account.loan_acc_num, "");
    strcpy(loan_account.assigned_employee, "");
    loan_account.credit_score = 0;
    loan_account.interest_rate = 0;
    loan_account.processed = 0;
    loan_account.accepted = 0;

    printf("\n\nLoan Application\n\n");

    // do {
        printf("\nLoan Type: \n1. Personal Loan \n2. Education Loan \n3. Car Loan \n4. Home Loan \n5. Other\n\nChoice: ");
        scanf("%u", &loan_account.type);

    //     if(loan_account.type > 5 || loan_account.type <= 0) {
    //         printf("\nInvalid Loan Type!\n");
    //     }
    // } while(loan_account.type > 5 || loan_account.type <= 0);

    // do {
        printf("\nLoan Amount: "); 
        scanf("%f", &loan_account.loan_amount);
        
    //     if(loan_account.loan_amount <= 0) {
    //         printf("\nInvalid Loan Amount!\n");
    //     }

    //     printf("%f\n", loan_account.loan_amount);
    // } while(loan_account.loan_amount <= 0);

    // do {
        printf("\nLoan Repayment Duration (Months): "); 
        scanf("%u", &loan_account.loan_repayment_duration_months);

    //     if(loan_account.loan_repayment_duration_months <= 0) {
    //         printf("\nInvalid Loan Repayment Duration!\n");
    //     }
    // } while(loan_account.loan_repayment_duration_months <= 0);

    // do {
        printf("\nYour Annual Income: "); 
        scanf("%u", &loan_account.annual_income);

    //     if(loan_account.annual_income <= 0) {
    //         printf("\nInvalid Annual Income!\n");
    //     }
    // } while(loan_account.annual_income <= 0);

    
    write(*client_fd, &loan_account, sizeof(struct Loan_Account_S));
    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("\n\n***Loan Application Submission Failed!***");
    } else if(status == 0) {
        printf("\n\n***Loan Application Submission Failed (Invalid Data Provided)!***");
    } else if(status == 1) {
        printf("\n\n***Loan Application Sumitted***");
    }
}

void customer_change_password(int *client_fd) {
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

void customer_add_feedback(int *client_fd) {
    char feedback_content[FEEDBACK_CONTENT_LEN];
    printf("\nEnter Your Feedback: \n"); scanf(" %[^\n]", feedback_content);
    write(*client_fd, &feedback_content, sizeof(feedback_content));

    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("\nFailed to Submit Feedback!\n");
    }else if(status == 1) {
        printf("\nFeedback Submitted Successfully.\nThank You!\n");
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
            customer_transfer_funds(client_fd);
            break;

        case 5:
            customer_apply_for_loan(client_fd);
            break;
        case 6:
            customer_change_password(client_fd);
            break;
        case 7:
            customer_add_feedback(client_fd);
            break;
        case 8:
            customer_view_transaction_history(client_fd);
            break;
        case 9:
            // Logout
            return 1;
        }

    } while(operation > 0 && operation < 10 );

    return -1;
}