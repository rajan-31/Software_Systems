
/* 
struct Employee_S *list_all_employees(int *num_employees) {
    struct Employee_S *all_employees = NULL;
    int count = 0;

    int fd = open("./data/employee.dat", O_RDONLY);
    if (fd == -1) {
        perror("Error opening employee.dat file");
        return NULL;
    }
    

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;

    fcntl(fd, F_SETLKW, &lock);

    struct Employee_S temp;    
    while (read(fd, &temp, sizeof(struct Employee_S)) > 0) {
        struct Employee_S *new_all_employees = realloc(all_employees, sizeof(struct Employee_S) * (count + 1));
        if (new_all_employees == NULL) {
            perror("Error reallocating memory");
            free(all_employees);  // Free the existing memory if realloc fails
            close(fd);
            return NULL;
        }

        all_employees = new_all_employees;
        // Copy the new employee data to the array
        all_employees[count] = temp;
        count++;
    }

    // Unlock the file
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error unlocking file");
    }

    // Close the file
    close(fd);

    // Set the number of employees
    *num_employees = count;

    // Return the dynamically allocated array of employees
    return all_employees;
}
*/

/* 
int count_num = 0;
struct Employee_S *all_employees = (struct Employee_S *) list_all_records(EMPLOYEE_E, &count_num);

for(int i=0; i<=count_num-1; i++) {
    printf("%s\n", all_employees[i].username);
}
 */
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

