#define USERNAME_LEN 100
#define PASSWORD_LEN 100
#define FULLNAME_LEN 100

#define ACCOUNT_NUM_LEN 100

#define FEEDBACK_CONTENT_LEN 1000

#define TRANSACTION_ID_LENGTH 37

// XXXX_E, to start actual values from 1
enum role_type {XXXX_E, ADMIN_E, MANAGER_E, EMPLOYEE_E, CUSTOMER_E};
enum transaction_type {CREDIT_E, DEBIT_E};

struct Session_S {
    int socket_fd;
    char username[USERNAME_LEN];
};

struct Admin_S {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    char fullname[FULLNAME_LEN];
};

struct Employee_S {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    char fullname[FULLNAME_LEN];
    enum role_type role;
};

struct Customer_S {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    char fullname[FULLNAME_LEN];
    int active;
    
    char savings_acc_num[ACCOUNT_NUM_LEN];
    float savings_acc_balance;
};

struct Loan_Acoount_S {
    char username[USERNAME_LEN];

    char loan_acc_num[ACCOUNT_NUM_LEN];
    float loan_acc_balance;

    char assigned_employee[USERNAME_LEN];

    int processed;
    int accepted;
};

struct Transaction_S {
    char transaction_id[TRANSACTION_ID_LENGTH];
    time_t timestamp;
    float amount;
    enum transaction_type t_type;

    char payer[USERNAME_LEN];
    char payee[USERNAME_LEN];

    float payer_balance;
    float payee_balance;
};

struct Feedback_S {
    char username[USERNAME_LEN];
    char content[FEEDBACK_CONTENT_LEN];
};

