# User Space Threads

> [!NOTE]
> To change the stack is only needed to alloc space for future
> stack operations, as previous values are stored in the same
> address and accessed by address so it is not needed to have it
> duplicated. Thread_init or the function that init a thread should
> alloc some amount of stack memory, set that as the new stack and
> restore the previous stack before return.
