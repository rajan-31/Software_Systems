#ifndef IGNORE_IMPORTS
#include "./schema.h"
#include "./console.h"
#include <fcntl.h>
#include <unistd.h>
#endif

// -1: failed
float customer_view_balance(int *client_socket, char *username) {
    int fd = open("./data/customer.dat", O_RDONLY);
    if (fd == -1) {
        perror("Error opening customer.dat file");
        return -1;
    }
    
    float result = -1;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Customer_S temp;    
    while (read(fd, &temp, sizeof(struct Customer_S)) > 0) {
        if (strcmp(temp.username, username) == 0) {
            result = temp.savings_acc_balance;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    close(fd);
    return result;
}

// -1: invalid username, 0: wrong password, 1: verified
int customer_verify_password(char *username, char *password, struct Customer_S *customer_data) {
    int fd = open("./data/customer.dat", O_RDONLY);
    if (fd == -1) {
        perror("Error opening customer.dat file");
        return -1;
    }
    
    int result = -1;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Customer_S temp;    
    while (read(fd, &temp, sizeof(struct Customer_S)) > 0) {
        if (strcmp(temp.username, username) == 0) {
            if (strcmp(temp.password, password) == 0) {
                strcpy(customer_data->username, temp.username);
                strcpy(customer_data->password, temp.password);
                strcpy(customer_data->fullname, temp.fullname);

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

void customer_add_to_transaction_history(struct Transaction_S tx) {
    int fd = open("./data/transaction.dat", O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Error opening transaction.dat file");
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = 0;
    lock.l_len = sizeof(struct Transaction_S);
    fcntl(fd, F_SETLKW, &lock);

    write(fd, &tx, sizeof(struct Transaction_S));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}

// -2: insufficient balance, -1: failed, 1: balance changed
int customer_change_balance(char *username, float amount) {
    int fd = open("./data/customer.dat", O_RDWR);
    if (fd == -1) {
        perror("Error opening customer.dat file");
        return -1;
    }

    // =======================================
    // read => find index
    int result = -1;
    int idx = -1;
    int flag = 0;   // set if we find a match
    

    struct Customer_S temp;
    while (read(fd, &temp, sizeof(struct Customer_S)) > 0) {
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
        lock.l_start = idx * sizeof(struct Customer_S);
        lock.l_len = sizeof(struct Customer_S);

        fcntl(fd, F_SETLKW, &lock);


        lseek(fd, idx * sizeof(struct Customer_S), SEEK_SET);
        read(fd, &temp, sizeof(struct Customer_S));

        lseek(fd, -1 * sizeof(struct Customer_S), SEEK_CUR);

        if(temp.savings_acc_balance + amount >= 0) {
            temp.savings_acc_balance += amount;
            write(fd, &temp, sizeof(struct Customer_S));
            result = 1;
        } else {
            result = -2;
        }


        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
    }

    if(result == 1) {
        struct Transaction_S tx;
        
        strcpy(tx.transaction_id, gen_uuid());
        tx.timestamp = time(NULL);
        if(amount < 0) {
            tx.amount = -1 * amount;
            tx.t_type = DEBIT_E;
        } else {
            tx.amount = amount;
            tx.t_type = CREDIT_E;
        }
        
        strcpy(tx.payer, username);
        strcpy(tx.payee, username);

        tx.payer_balance = temp.savings_acc_balance;
        tx.payee_balance = temp.savings_acc_balance;

        customer_add_to_transaction_history(tx);
    }

    close(fd);
    return result;
}

//-2: insufficient balance, -1: failed, 0: invalid amount, 1: amount withdrawn
int customer_withdraw_money(int *client_socket, char *username) {
    float amount_to_withdraw = 0;
    read(*client_socket, &amount_to_withdraw, sizeof(amount_to_withdraw));

    if(amount_to_withdraw <= 0) {
        return 0;
    }

    return customer_change_balance(username, (-1*amount_to_withdraw));
}

// -1: failed, 0: invalid amount, 1: amount withdrawn
int customer_deposit_money(int *client_socket, char *username) {
    float amount_to_withdraw = 0;
    read(*client_socket, &amount_to_withdraw, sizeof(amount_to_withdraw));
    if(amount_to_withdraw <= 0) {
        return 0;
    }

    return customer_change_balance(username, amount_to_withdraw);
}


// =======================================

// -1: failed
int customer_lock_account_by_username(int fd, char *username, struct flock *lock) {
    int record_pos = -1;
    struct Customer_S record;

    lseek(fd, 0, SEEK_SET);
    while (read(fd, &record, sizeof(struct Customer_S)) > 0) {
        record_pos++;
        if (strcmp(record.username, username) == 0) {
            lock->l_type = F_WRLCK;
            lock->l_whence = SEEK_SET;
            lock->l_start = record_pos * sizeof(struct Customer_S);
            lock->l_len = sizeof(struct Customer_S);

            if (fcntl(fd, F_SETLKW, lock) == -1) {
                return -1;
            }

            return record_pos;
        }
    }
    printf("no record matched\n");
    return -1;
}

void customer_release_lock_by_position(int fd, int record_pos, struct flock *lock) {
    lock->l_type = F_UNLCK;
    lock->l_whence = SEEK_SET;
    lock->l_start = record_pos * sizeof(struct Customer_S);
    lock->l_len = sizeof(struct Customer_S);
    fcntl(fd, F_SETLKW, lock);  // Release lock
}

// -2: insufficient balance, 0: Invalid Amount; -1: failed, 1: transferred
int customer_transfer_funds(int *client_socket, char *username) {
    char payer_u[USERNAME_LEN]; strcpy(payer_u, username);
    char payee_u[USERNAME_LEN];
    read(*client_socket, &payee_u, sizeof(payee_u));

    float amount_to_transfer = 0;
    read(*client_socket, &amount_to_transfer, sizeof(amount_to_transfer));

    if(amount_to_transfer <= 0)
        return 0;

    // =======================================

    int fd = open("./data/customer.dat", O_RDWR);

    struct flock lock;

    /* 
        To avoid starvation keep fix order while locking => Consistent Locking 
    */
    int first_pos = -1, second_pos = -1;
    
    char first_u[USERNAME_LEN]; 
    strcpy(first_u, strcmp(payer_u, payee_u) < 0 ? payer_u : payee_u);
    char second_u[USERNAME_LEN]; 
    strcpy(second_u, strcmp(payer_u, payee_u) < 0 ? payee_u : payer_u);
    
    // lock first_u
    first_pos = customer_lock_account_by_username(fd, first_u, &lock);
    if (first_pos == -1) {
        printf("Error locking first account.\n");
        close(fd);
        return -1;
    }

    // lock second_u
    second_pos = customer_lock_account_by_username(fd, second_u, &lock);
    if (second_pos == -1) {
        printf("Error locking second account.\n");
        customer_release_lock_by_position(fd, first_pos, &lock);
        close(fd);
        return -1;
    }

    // =======================================
    int payer_pos = strcmp(payer_u, payee_u) < 0 ? first_pos : second_pos;
    int payee_pos = strcmp(payer_u, payee_u) < 0 ? second_pos : first_pos;


    struct Customer_S payer_record, payee_record;

    lseek(fd, payer_pos * sizeof(struct Customer_S), SEEK_SET);
    read(fd, &payer_record, sizeof(struct Customer_S));

    lseek(fd, payee_pos * sizeof(struct Customer_S), SEEK_SET);
    read(fd, &payee_record, sizeof(struct Customer_S));

    int result = -1;
    if(payer_record.savings_acc_balance < amount_to_transfer) {
        result = -2;
    } else {
        payer_record.savings_acc_balance-=amount_to_transfer;
        payee_record.savings_acc_balance+=amount_to_transfer;

        lseek(fd, payer_pos * sizeof(struct Customer_S), SEEK_SET);
        write(fd, &payer_record, sizeof(struct Customer_S));

        lseek(fd, payee_pos * sizeof(struct Customer_S), SEEK_SET);
        write(fd, &payee_record, sizeof(struct Customer_S));

        result = 1;

        // =======================================
        
        struct Transaction_S tx;

        strcpy(tx.transaction_id, gen_uuid());
        tx.timestamp = time(NULL);
        tx.amount = amount_to_transfer;
        tx.t_type = DEBIT_E;

        strcpy(tx.payer, payer_u);
        strcpy(tx.payee, payee_u);

        tx.payer_balance = payer_record.savings_acc_balance;
        tx.payee_balance = payee_record.savings_acc_balance;

        customer_add_to_transaction_history(tx);
    }

    customer_release_lock_by_position(fd, first_pos, &lock);
    customer_release_lock_by_position(fd, second_pos, &lock);
    close(fd);

    return result;
}

void customer_view_transaction_history(int *client_socket, char *username) {
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
        if(strcmp(temp.payee, username) == 0 || strcmp(temp.payer, username) == 0) {
            if(strcmp(temp.payer, temp.payee) == 0) {
                to_include = 1;
            } else if(strcmp(temp.payer, username) == 0) {
                temp.payee_balance = -1;
                to_include = 1;
            } else if(strcmp(temp.payee, username) == 0) {
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

int customer_apply_for_loan(int *client_socket, char *username) {
    struct Loan_Account_S loan_account;
    read(*client_socket, &loan_account, sizeof(loan_account));

    strcpy(loan_account.username, username);
    strcpy(loan_account.loan_acc_num, gen_uuid());

    int fd = open("./data/loan.dat", O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Error opening loan.dat file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = 0;
    lock.l_len = sizeof(struct Loan_Account_S);
    fcntl(fd, F_SETLKW, &lock);

    write(fd, &loan_account, sizeof(struct Loan_Account_S));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    return 1;
}

int customer_change_password(int *client_socket, char *username) {
    int is_password_confirmed = -1;
    read(*client_socket, &is_password_confirmed, sizeof(is_password_confirmed));

    if(is_password_confirmed != 1)
        return 0;

    
    // =======================================
    char new_password[PASSWORD_LEN];
    read(*client_socket, &new_password, sizeof(new_password));

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
        lock.l_start = idx * sizeof(struct Customer_S);
        lock.l_len = sizeof(struct Customer_S);

        fcntl(fd, F_SETLKW, &lock);


        lseek(fd, idx * sizeof(struct Customer_S), SEEK_SET);
        read(fd, &temp, sizeof(struct Customer_S));

        lseek(fd, -1 * sizeof(struct Customer_S), SEEK_CUR);

        strcpy(temp.password, new_password);
        write(fd, &temp, sizeof(struct Customer_S));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
        result = 1;
    }

    close(fd);

    return result;
}


int customer_add_feedback(int *client_socket, char *username) {
    char feedback_content[FEEDBACK_CONTENT_LEN];
    read(*client_socket, &feedback_content, sizeof(feedback_content));

    int fd = open("./data/feedback.dat", O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Error opening feedback.dat file");
        return -1;
    }

    struct Feedback_S fb;
    strcpy(fb.username, username);
    strcpy(fb.content, feedback_content);
    fb.seen = 0;

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = 0;
    lock.l_len = sizeof(struct Feedback_S);
    fcntl(fd, F_SETLKW, &lock);

    write(fd, &fb, sizeof(struct Feedback_S));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    return 1;
}

// =======================================

void handle_customer_menu(int *client_socket, struct Customer_S *customer_data) {
    write(*client_socket, CUSTOMER_MENU_MSG, strlen(CUSTOMER_MENU_MSG));

    int operation=1;
    int status = -1;
    while(read(*client_socket, &operation, sizeof(operation)) && operation > 0 && operation < 10 ) {
        switch (operation)
        {
        case 1:
            float savings_acc_balance = customer_view_balance(client_socket, customer_data->username);
            write(*client_socket, &savings_acc_balance, sizeof(savings_acc_balance));
            break;

        case 2:
            status = customer_withdraw_money(client_socket, customer_data->username);
            write(*client_socket, &status, sizeof(status));
            break;

        case 3:
            status = customer_deposit_money(client_socket, customer_data->username);
            write(*client_socket, &status, sizeof(status));
            break;
            
        case 4:
            status = customer_transfer_funds(client_socket, customer_data->username);
            write(*client_socket, &status, sizeof(status));
            break;

        case 5:
            status = customer_apply_for_loan(client_socket, customer_data->username);
            write(*client_socket, &status, sizeof(status));
            break;
        case 6:
            status = customer_change_password(client_socket, customer_data->username);
            write(*client_socket, &status, sizeof(status));
            break;
        case 7:
            status = customer_add_feedback(client_socket, customer_data->username);
            write(*client_socket, &status, sizeof(status));
            break;
        case 8:
            customer_view_transaction_history(client_socket, customer_data->username);
            break;
        case 9:
            // Logout
            break;
        }

    }
}

void handle_customer_login(int *client_socket, char *username) {
    write(*client_socket, MAIN_MENU_ASK_PASSWORD, strlen(MAIN_MENU_ASK_PASSWORD));

    char password[PASSWORD_LEN];
    read(*client_socket, &password, sizeof(password));

    struct Customer_S customer_data;
    int customer_verify_password_status = customer_verify_password(username, password, &customer_data);
    write(*client_socket, &customer_verify_password_status, sizeof(customer_verify_password_status));

    if(customer_verify_password_status == 1) {
        handle_customer_menu(client_socket, &customer_data);
    }
}