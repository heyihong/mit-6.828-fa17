# Homework 8

1. The breakpoint may (or may not) be triggered before you even run uthread. How could that happen?

Answer: Some process created during starting kernel may have some instruction starts at the same address, like sh.

2. What address is 0xd8, which sits on the top of the stack of next_thread?

Answer: The function pointer for the user thread to execute. 

