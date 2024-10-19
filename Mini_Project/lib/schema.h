#define USERNAME_LEN 100
#define PASSWORD_LEN 100
#define FULLNAME_LEN 100

#define ACCOUNT_NUM_LEN 100

#define FEEDBACK_CONTENT_LEN 1000

#define TRANSACTION_ID_LENGTH 37

// XXXX_E, to start actual values from 1
enum role_type {XXXX_ROLE_E, ADMIN_E, MANAGER_E, EMPLOYEE_E, CUSTOMER_E};

enum transaction_type {CREDIT_E, DEBIT_E};

// XXXX_E, to start actual values from 1
enum loan_type {XXXX_LOAN_E, PERSONAL_LOAN_E, EDUCATION_LOAN_E, VEHICLE_LOAN_E, HOME_LOAN_E, OTHER_LOAN_E};

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

struct Loan_Account_S {
    char username[USERNAME_LEN];
    char loan_acc_num[ACCOUNT_NUM_LEN];

    // ask user
    enum loan_type type;
    float loan_amount;
    unsigned int loan_repayment_duration_months;
    unsigned int annual_income;

    // ask manager
    char assigned_employee[USERNAME_LEN];

    // ask employee
    unsigned int credit_score;
    unsigned int interest_rate;
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
    int seen;
};

