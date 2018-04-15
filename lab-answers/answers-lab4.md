# Questions

1. Compare kern/mpentry.S side by side with boot/boot.S. Bearing in mind that kern/mpentry.S is compiled and linked to run above KERNBASE just like everything else in the kernel, what is the purpose of macro MPBOOTPHYS? Why is it necessary in kern/mpentry.S but not in boot/boot.S? In other words, what could go wrong if it were omitted in kern/mpentry.S? 

Answer: The purpose of MPBOOTPHYS is to calculate the correct memory address during execution. For kern/mpentry.S, the load address of kern/mpentry.S is MPENTRY_PADDR while the link address of kern/mpentry.S is not. For boot/boot.S, both load address and link address are same. If it were omitted, it will execute unkown memory at its link address. 

2. It seems that using the big kernel lock guarantees that only one CPU can run the kernel code at a time. Why do we still need separate kernel stacks for each CPU? Describe a scenario in which using a shared kernel stack will go wrong, even with the protection of the big kernel lock.

Answer: If using shared kernel stack, when two processes have system calls simulateneously, the trapframe will be overwrote by the latter one, the former one will go wrong.

3. In your implementation of env_run() you should have called lcr3(). Before and after the call to lcr3(), your code makes references (at least it should) to the variable e, the argument to env_run. Upon loading the %cr3 register, the addressing context used by the MMU is instantly changed. But a virtual address (namely e) has meaning relative to a given address context--the address context specifies the physical address to which the virtual address maps. Why can the pointer e be dereferenced both before and after the addressing switch?

Answer: Because all different environments have same kernel address space mapping and all memory used by env_run are in kernel address space.

4. Whenever the kernel switches from one environment to another, it must ensure the old environment's registers are saved so they can be restored properly later. Why? Where does this happen? 

Answer: Because the environment could not execute correctly if its registers changed in switch. The save happens in trap() function `curenv->env_tf = *tf`.