## Run

### Server

```bash
gcc server.c -o server -largon2
./server
```

### Client

```bash
gcc client.c -o client -largon2
./client
```

## Features

- Multithreading, to handle multple concurrent users
- Hashing password with "Argon2", which is secure than bcrypt, sha256 (not built for pass hashing), etc.
- 2PL with consistent locking (locks in same order) for transactions
    - Get all locks in fix order, modify, release all locks
    - It avoids deadlock
    - It avoids circular dependency
- Journal (Write-Ahead Logging)
    - To log transfer of funds before commiting
    - To use it in case of system failure

## Potential Improvements

- Use random salt for password hashing and save it in user data
    - Otherwise password is vulnerable to rainbow table attacks, having salt does not fully eliminate the risk but it make it more costly (time and power wise) to guess a password
- Utilize logging to maintain consistency of data, in case of system crash


## Learnings

- Important to initialize struct with values or memset, to avoid garbage values getting assigned (causing problems)
- malloc vs calloc
    - malloc will give garbage values (reuse dirty memory)
    - calloc zeroes out values

    - if you are sure you are going to initialize all values, prefer malloc
    else calloc

    - there is more to it, if look into finer details about both


<!-- 
- We can assign cpu core to thread manually (with pthread_setaffinity_np(...))
- But, generally it's preferred to let OS handle it, to avoid hurting performance and keep code more portable

- Uing 2PL with consistent locking (locks in same order) for transactions, get all locks, modify, release all locks

- Handle circular dependency

- Used argon2 (argon2id) for password hashing, 

- Currently I have not used random hash and stored in database, but that's what should be done, otherwise password is vulnerable to rainbow table attack, having salt does not fully eliminate the risk but it make it more costly (time and power wise) to guess a password

- Journal (Write-Ahead Logging)
    to log transfer of funds before commiting
    to use it in case of system failure


---

To do
- [*] hide pass input
- [*] password hashing
- [*] only active cus can login
- [ ] ~~should not assign another manager to loan~~
- [*] transaction history can see all, should see own only
- [*] any employee if select role manager, he shouldn't be able to login, fix this

- [ ] only assigned employee should be able to proccess and accept/reject loan
- [*] you can transfer to yourself, handle this
- [*] duplicate emp, cus add, handle this
- [*] ask again after wrong password
- [*] ACID
- [*] admin - list customers, list employees
- [*] manager - list customers, list employees
- [*] customer - list loan applications
- [*] manager - view loan app


---

- Important to initialize struct with values or memset, to avoid garbage values causing problems
- malloc vs calloc
    malloc will give garbage values (reuse dirty memory)
    calloc zeroes out values

    if you are sure you are going to initialize all values, prefer malloc
    else calloc

    there is more to it, if look into finer details about both
-  -->


