#define USERNAME_LEN 100
#define PASSWORD_LEN 100
#define FULLNAME_LEN 100

#define ACCOUNT_NUM_LEN 100

#define FEEDBACK_CONTENT_LEN 1000

// XXXX_E, to start actual values from 1
enum role_type {XXXX_E, ADMIN_E, MANAGER_E, EMPLOYEE_E, CUSTOMER_E};

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
    int accepted;
};

struct Feedback_S {
    char username[USERNAME_LEN];
    char content[FEEDBACK_CONTENT_LEN];
};