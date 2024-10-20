#include <stdio.h>    // printf, perror, scanf, getchar
#include <unistd.h>   // close, lseek, read, write, SEEK_SET
#include <fcntl.h>    // open, fcntl, O_RDONLY, O_RDWR, F_SETLKW, F_RDLCK, F_WRLCK, F_UNLCK, struct flock
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "./lib/schema.h"
#include "./lib/common.h"

void clear_data_and_init_admin() {
    int fd1=open("./data/admin.dat", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd1==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    struct Admin_S admin_data;
    int num_records1=3;
    for(int i=1; i<=num_records1; i++) {
        sprintf(admin_data.username, "admin%d", i);
        strcpy(admin_data.password, "123456");
        sprintf(admin_data.fullname, "Admin %d", i);

        write(fd1, &admin_data, sizeof(struct Admin_S));
    }


    close(fd1);
}

void clear_data_and_init_employee() {
    int fd2=open("./data/employee.dat", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd2==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    struct Employee_S employee_data;
    int num_records2=3;
    for(int i=1; i<=num_records2; i++) {
        sprintf(employee_data.username, "emp%d", i);
        strcpy(employee_data.password, "123456");
        sprintf(employee_data.fullname, "Emp %d", i);
        employee_data.role = EMPLOYEE_E;

        write(fd2, &employee_data, sizeof(struct Employee_S));
    }

    close(fd2);
}

void clear_data_and_init_customer() {
    int fd3=open("./data/customer.dat", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd3==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    struct Customer_S customer_data;
    int num_records3=3;
    for(int i=1; i<=num_records3; i++) {
        sprintf(customer_data.username, "cus%d", i);
        strcpy(customer_data.password, "123456");
        sprintf(customer_data.fullname, "Cus %d", i);
        customer_data.active = 0;
        strcpy(customer_data.savings_acc_num, "");
        customer_data.savings_acc_balance = 0;

        write(fd3, &customer_data, sizeof(struct Customer_S));
    }

    close(fd3);
}

void clear_data_and_init_transaction_history() {
    char filepath[] = "./data/transaction.dat";
    int num_records=3;
    struct Transaction_S data;

 
    int fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for(int i=1; i<=num_records; i++) {
        strcpy(data.transaction_id, gen_uuid());
        data.timestamp = time(NULL);
        data.amount = rand() % 100;
        data.t_type = rand() % 2;
        
        sprintf(data.payer, "cus%d", (rand() % 3) + 1);
        sprintf(data.payee, "cus%d", (rand() % 3) + 1);

        if(strcmp(data.payer, data.payee) == 0) {
            data.payer_balance = rand() % 1000;
            data.payee_balance = data.payer_balance;
        } else {
            data.payer_balance = rand() % 1000;
            data.payee_balance = rand() % 1000;
        }


        write(fd, &data, sizeof(struct Transaction_S));
    }

    close(fd);
}

void clear_data_loan() {
    int fd = open("./data/loan.dat", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void clear_data_transaction_history() {
    int fd = open("./data/transaction.dat", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void clear_data_feedback() {
    int fd = open("./data/feedback.dat", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void print_all() {
    int fd1=open("./data/admin.dat", O_RDONLY);
    if(fd1==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("======== Admins ========\n");
    printf("\nuname\tpass\tfname\n");
    struct Admin_S admin_data;
    while(read(fd1, &admin_data, sizeof(struct Admin_S))) {
        printf("%s\t%s\t'%s'\n", admin_data.username, admin_data.password, admin_data.fullname);
    }


    int fd2=open("./data/employee.dat", O_RDONLY);
    if(fd2==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("\n======== Employees ========\n");
    printf("\nuname\tpass\tfname\trole\n");
    struct Employee_S employee_data;
    while(read(fd2, &employee_data, sizeof(struct Employee_S))) {
        printf("%s\t%s\t%s\t%d\n", employee_data.username, employee_data.password, employee_data.fullname, employee_data.role);
    }


    int fd3=open("./data/customer.dat", O_RDONLY);
    if(fd3==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("\n======== Customers ========\n");
    printf("\nuname\tpass\tfname\tactive\ts_acc_num\ts_acc_bal\n");
    struct Customer_S customer_data;
    while(read(fd3, &customer_data, sizeof(struct Customer_S))) {
        printf("%s\t%s\t%s\t%d\t'%s'\t\t%f\n", customer_data.username, customer_data.password, customer_data.fullname, customer_data.active, customer_data.savings_acc_num, customer_data.savings_acc_balance);
    }


    int fd4=open("./data/transaction.dat", O_RDONLY);
    if(fd4==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("\n======== Transaction History ========\n");
    printf("\n%-37s %-10s %-8s %-14s %-6s %-20s %-20s %-17s %-17s\n",
        "Transaction ID", "Date", "Time", "Amount (₹)", "Type", "Payer", "Payee", "Payer Balance (₹)", "Payee Balance (₹)"
    );

    struct Transaction_S tx;
    while(read(fd4, &tx, sizeof(struct Transaction_S))) {
        char time_buffer[20];
        struct tm *time_info = localtime(&tx.timestamp);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);

        printf("%-37s %-19s %-12.2f %-6s %-20s %-20s %-17.2f %-17.2f\n",
            tx.transaction_id,
            time_buffer,
            tx.amount,
            tx.t_type == CREDIT_E ? "CREDIT" : "DEBIT",
            tx.payer,
            tx.payee,
            tx.payer_balance,
            tx.payee_balance
        );
    }


    int fd5=open("./data/loan.dat", O_RDONLY);
    if(fd5==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    printf("\n======== Loan Application ========\n");
    printf("%-15s %-37s %-14s %-12s %-10s %-10s %-15s %-10s %-10s %-10s %-10s\n",
        "username", "acc", "type", "amount", "duration", "annual_inc", "employee", "credit_sc", "interest", "processed", "accepted"
    );
    struct Loan_Account_S loan_account;
    while (read(fd5, &loan_account, sizeof(struct Loan_Account_S))) {
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
            loan_account.accepted);
    }


    printf("\n======== Feedbacks ========\n");

    int fd6=open("./data/feedback.dat", O_RDONLY);
    if(fd6==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("\nuname\tseen\tcontent\n");
    struct Feedback_S feedback_data;
    while(read(fd6, &feedback_data, sizeof(struct Feedback_S))) {
        printf("%s\t%d\t%s\n", feedback_data.username, feedback_data.seen, feedback_data.content);
    }


    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    close(fd5);
    close(fd6);
}

int main() {
    char *INIT_OPTIONS = \
    "What to do?\n\n"
    "1. Clear data and init admin\n"
    "2. Clear data and init employee\n"
    "3. Clear data and init customer\n"
    "4. Clear data and init transaction history\n"
    "5. Clear data loan\n"
    "6. Clear data transaction history\n"
    "7. Clear data feedback\n"
    "*. Print all\n"
    "\nChoice: ";

    printf("%s", INIT_OPTIONS);
    int choice; scanf("%d", &choice);


    switch (choice) {
    case 1:
        clear_data_and_init_admin();
        break;

    case 2:
        clear_data_and_init_employee();
        break;

    case 3:
        clear_data_and_init_customer();
        break;
    case 4:
        clear_data_and_init_transaction_history();
        break;
    case 5:
        clear_data_loan();
        break;

    case 6:
        clear_data_transaction_history();
        break;

    case 7:
        clear_data_feedback();
        break;
    
    default:
        break;
    }
    
    print_all();

    return 0;    
}