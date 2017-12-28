# Homework 5
How to add system call to xv6 is pretty similar to hw3. The challenge here is to upcall the handler from kernel and the solution here is not general. It sets trapframe eip to handler and push return address(i.e. the original eip value) to stack. It works in alarmtest but since it doesn't restore registers after invoking handler, it will not work if registers' value changes.
