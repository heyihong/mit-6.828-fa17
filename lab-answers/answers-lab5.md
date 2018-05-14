# Questions

1. Do you have to do anything else to ensure that this I/O privilege setting is saved and restored properly when you subsequently switch from one environment to another? Why?

Answer: No. Because trapframe will be saved and restored by kernel during switch and I/O priviledge settings is recorded in eflags field of trapframe.