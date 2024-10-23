#ifndef IGNORE_IMPORTS
#include <argon2.h>
#endif


#define SALT_LEN 16
#define HASH_LEN 32  // bytes


// Function obtained from internet (https://stackoverflow.com/a/71826534/13460667)
// Purpose: Generates a universally unique identifier (UUID)
char* gen_uuid() {
    char v[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    //3fb17ebc-bc38-4939-bc8b-74f2443281d4
    //8 dash 4 dash 4 dash 4 dash 12
    static char buf[37] = {0};

    //gen random for all places
    for(int i = 0; i < 36; ++i) {
        buf[i] = v[rand()%16];
    }

    //put dashes in place
    buf[8] = '-';
    buf[13] = '-';
    buf[18] = '-';
    buf[23] = '-';

    //needs end byte
    buf[36] = '\0';

    return buf;
}

// =======================================

void gen_salt(char *salt) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int i;

    srand(time(NULL));

    for (i = 0; i < SALT_LEN; i++) {
        salt[i] = charset[rand() % (sizeof(charset) - 1)];
    }

    salt[SALT_LEN] = '\0';
}


// hash the password using Argon2
char* hash_password(const char *password, const char *salt) {
    unsigned int t_cost = 2;            // Number of iterations
    unsigned int m_cost = (1 << 16);    // Memory cost (64 MiB)
    unsigned int parallelism = 1;       // Number of threads
    size_t hash_len = HASH_LEN;      // Length of the output hash in bytes

    // Calculate the length required for the output buffer
    size_t output_size = argon2_encodedlen(t_cost, m_cost, parallelism, SALT_LEN, HASH_LEN, Argon2_id);
    
    // Allocate memory for the output buffer
    char *output_hash = malloc(output_size);
    if (!output_hash) {
        printf("Memory allocation failed!\n");
        return NULL; // Return NULL if memory allocation fails
    }

    // Hash the password using Argon2id and encode the result
    int result = argon2id_hash_encoded(
        t_cost, m_cost, parallelism, 
        password, strlen(password), 
        salt, SALT_LEN, hash_len, output_hash, output_size
    );

    if (result != ARGON2_OK) {
        printf("Error hashing password: %s\n", argon2_error_message(result));
        free(output_hash); // Free memory in case of error
        return NULL; // Return NULL if hashing fails
    }

    return output_hash; // Return the pointer to the output hash
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Do nothing, just consume characters
    }
}


// =======================================

/* 
int count_num = 0;
struct Employee_S *all_employees = (struct Employee_S *) list_all_records(EMPLOYEE_E, &count_num);

for(int i=0; i<=count_num-1; i++) {
    printf("%s\n", all_employees[i].username);
}
 */
/*
void *list_all_records(enum role_type role, int *num_records) {
    void *all_records = NULL;
    int count = 0;

    const char *filename = NULL;

    // Set the filename based on the role
    if(role == EMPLOYEE_E) {
        filename = "./data/employee.dat";
    } else if(role == ADMIN_E) {
        filename = "./data/admin.dat";
    } else if(role == CUSTOMER_E) {
        filename = "./data/customer.dat";
    }

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return NULL;
    }

    // Lock the file for reading
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error locking file");
        close(fd);
        return NULL;
    }

    // Temporary buffer for reading each record
    void *temp = NULL;
    ssize_t record_size = 0;

    // Determine the structure and size based on the role
    if(role == EMPLOYEE_E) {
        record_size = sizeof(struct Employee_S);
    } else if(role == ADMIN_E) {
        record_size = sizeof(struct Admin_S);
    } else if(role == CUSTOMER_E) {
        record_size = sizeof(struct Customer_S);
    }

    // Allocate temporary memory for the structure
    temp = malloc(record_size);
    if (temp == NULL) {
        perror("Error allocating memory");
        close(fd);
        return NULL;
    }

    // Read records from the file
    while (read(fd, temp, record_size) == record_size) {
        void *new_all_records = realloc(all_records, record_size * (count + 1));
        if (new_all_records == NULL) {
            perror("Error reallocating memory");
            free(all_records);  // Free the existing memory if realloc fails
            free(temp);         // Free the temp buffer
            close(fd);
            return NULL;
        }

        all_records = new_all_records;
        // Copy the read record into the all_records array
        memcpy((char*)all_records + (count * record_size), temp, record_size);
        count++;
    }

    // Unlock the file
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error unlocking file");
    }

    // Close the file
    close(fd);

    // Set the number of records
    *num_records = count;

    // Free the temporary buffer
    free(temp);

    // Return the dynamically allocated array of records
    return all_records;
}
*/
