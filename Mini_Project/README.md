- We can assign cpu core to thread manually (with pthread_setaffinity_np(...))
- But, generally it's preferred to let OS handle it, to avoid hurting performance and keep code more portable

- uing 2PL which is consistent (locks in same order) for transaction, get all locks, modify, release all locks

- handle circular dependency

---

To do

- password hashing
- only active cus can login
- should not assign another manager to loan
- transaction history can see all, should see own only
- any employee if select role manager, he si able to login, fix this

- only assigned employee should be able to proccess and accept/reject loan