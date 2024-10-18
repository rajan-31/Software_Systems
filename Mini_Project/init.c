#include <stdio.h>    // printf, perror, scanf, getchar
#include <unistd.h>   // close, lseek, read, write, SEEK_SET
#include <fcntl.h>    // open, fcntl, O_RDONLY, O_RDWR, F_SETLKW, F_RDLCK, F_WRLCK, F_UNLCK, struct flock
#include <string.h>
#include <stdlib.h>

#include "./lib/schema.h"
#include "./lib/common.h"

void clear_data_and_init() {
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


    close(fd1);
    close(fd2);
    close(fd3);
}

void print_all() {
    int fd1=open("./data/admin.dat", O_RDONLY);
    if(fd1==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("======== Admins ========\n");
    struct Admin_S admin_data;
    while(read(fd1, &admin_data, sizeof(struct Admin_S))) {
        printf("%s\t%s\t%s\n", admin_data.username, admin_data.password, admin_data.fullname);
    }


    int fd2=open("./data/employee.dat", O_RDONLY);
    if(fd2==-1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("\n======== Employees ========\n");
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
    struct Customer_S customer_data;
    while(read(fd3, &customer_data, sizeof(struct Customer_S))) {
        printf("%s\t%s\t%s\t%d\t%s\t%f\n", customer_data.username, customer_data.password, customer_data.fullname, customer_data.active, customer_data.savings_acc_num, customer_data.savings_acc_balance);
    }


    close(fd1);
    close(fd2);
    close(fd3);
}

int main() {
    printf("What to do? \n1. Clear data and init \n2. Print all \n3. Exit \n\nChoice: ");
    int choice;
    scanf("%d", &choice);

    if(choice == 1) {
        clear_data_and_init();
    } else if(choice == 2) {
        print_all();
    }

    return 0;    
}