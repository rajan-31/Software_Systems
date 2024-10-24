#ifndef IGNORE_IMPORTS
#include "./schema.h"
#include <stdio.h>
#include <stdlib.h>
#endif

void manager_activate_deactivate_customer(int *client_fd) {
    char customer_username[USERNAME_LEN];
    printf("Enter Customer Username: "); scanf("%s", customer_username);
    write(*client_fd, &customer_username, sizeof(customer_username));

    int do_activate = 1;
    printf("\n\n1. Deactivate\t2. Activate\nAction: "); scanf("%d", &do_activate);
    do_activate -= 1;
    write(*client_fd, &do_activate, sizeof(do_activate));


    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to %s Customer Account!\n", do_activate == 1 ? "Activate" : "Deactivate");
    } else if(status == 0) {
        printf("Invalid Customer Username!\n");
    } else if(status == 1) {
        printf("Succefully %s Customer Account\n", do_activate == 1 ? "Activated" : "Deactivated");
    }
}

void manager_assign_loan_application_process(int *client_fd) {
    char loan_acc_num[ACCOUNT_NUM_LEN];
    printf("Enter Loan Account Number: "); scanf("%s", loan_acc_num);
    write(*client_fd, &loan_acc_num, sizeof(loan_acc_num));

    char employee_username[USERNAME_LEN];
    printf("Enter Employee Username: "); scanf("%s", employee_username);
    write(*client_fd, &employee_username, sizeof(employee_username));


    int status = -1;
    read(*client_fd, &status, sizeof(status));

    if(status == -1) {
        printf("Failed to Assign Employee!\n");
    } else if(status == 0) {
        printf("Invalid Customer Username!\n");
    } else if(status == 1) {
        printf("Succefully Assigned Employee\n");
    }
}

void manager_review_customer_feedbacks(int *client_fd) {
    int feedback_size = 0;
    read(*client_fd, &feedback_size, sizeof(feedback_size));

    if(feedback_size <= 0) {
        printf("No Feedbacks!\n");
        return;
    }

    struct Feedback_S *feedback = (struct Feedback_S *) malloc(feedback_size * sizeof(struct Feedback_S));
    read(*client_fd, feedback, feedback_size * sizeof(struct Feedback_S));

    printf("\n%-15s %s\n", "Username", "Feedback");
    for(int i=0; i<=feedback_size-1; i++)
        printf("%-15s %s\n", feedback[i].username, feedback[i].content);
}

void manager_view_loan_applications(int *client_fd) {
    int loan_applications_size = 0;
    read(*client_fd, &loan_applications_size, sizeof(loan_applications_size));

    if(loan_applications_size == 0) {
        printf("No Loan Applications Found!\n");
        return;
    }

    struct Loan_Account_S *loan_applications = (struct Loan_Account_S *) malloc(loan_applications_size * sizeof(struct Loan_Account_S));
    read(*client_fd, loan_applications, loan_applications_size * sizeof(struct Loan_Account_S));

    printf("\n%-20s %-37s %-14s %-19s %-17s %-13s %-20s %-12s %-17s %-9s %-8s\n",
        "Username", "Account", "Type", "Amount (₹)", "Duration (Months)", "Annual Income", "Employee", "Credit Score", "Interest Rate (%)", "Processed", "Accepted"
    );

    for(int i=0;i<=loan_applications_size-1; i++) {
        printf("%-20s %-37s %-14s %-17.2f %-17d %-13d %-20s %-12d %-17d %-9s %-8s\n",
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
            loan_applications[i].processed == 1 ? "Yes" : "No",
            loan_applications[i].accepted == 1 ? "Yes" : "No"
        );
    }
}

int handle_manager_menu(int *client_fd, char *username, char *password){
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
            manager_activate_deactivate_customer(client_fd);
            break;

        case 3:
            admin_view_bank_employees(client_fd);
            break;

        case 4:
            manager_view_loan_applications(client_fd);
            break;

        case 5:
            manager_assign_loan_application_process(client_fd);
            break;

        case 6:
            manager_review_customer_feedbacks(client_fd);
            break;

        case 7:
            employee_change_password(client_fd);
            break;

        case 8:
            // Logout
            return 1;
        }

    } while(operation > 0 && operation < 9 );

    return -1;
}