#  Questions
1. What is the purpose of having an individual handler function for each exception/interrupt? (i.e., if all exceptions/interrupts were delivered to the same handler, what feature that exists in the current implementation could not be provided?)

Answer: The hardware could not infer the trapno safely when using the same handler. So if all exceptions/interrupts were delivered to the same handler, the kernel could not differentiate system call and breakpoint. 

2. Did you have to do anything to make the user/softint program behave correctly? The grade script expects it to produce a general protection fault (trap 13), but softint's code says int $14. Why should this produce interrupt vector 13? What happens if the kernel actually allows softint's int $14 instruction to invoke the kernel's page fault handler (which is interrupt vector 14)?))

Answer: It is related to priviledge. According to interrupt descriptor table set in trap_init, the DPL for int $14 is 0 while `int $14` is executed when its DPL is 3. The mismatch of the protection level leads to the hardware generates interrupts 13.

3. The break point test case will either generate a break point exception or a general protection fault depending on how you initialized the break point entry in the IDT (i.e., your call to SETGATE from trap_init). Why? How do you need to set it up in order to get the breakpoint exception to work as specified above and what incorrect setup would cause it to trigger a general protection fault?

Answer: The reason is the same as the answer of question 2. In order to trigger a break point exception, we need to set the interrupt descriptor of breakpoint to DPL_USER. 

4. What do you think is the point of these mechanisms, particularly in light of what the user/softint test program does?

Answer: I think the major goal of these mechanisms is to prevent the kernel from being corrupted when running and interacting with user programs by limiting the ways for user programs to enter the kernel mode.

