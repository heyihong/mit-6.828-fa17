# Exercise 2
It sets up an interrupt descriptor table and initializes various devices such as the VGA display.

# Exercise 3

Questions:
1. At what point does the processor start executing 32-bit code? What exactly causes the switch from 16- to 32-bit mode?
Answer: In boot.S, after executing instruction "ljmp    $PROT_MODE_CSEG, $protcseg", the processor start executing 32-bit code. Because it is only in this mode that software can access all the memory above 1MB in the processor's physical address space

2. What is the last instruction of the boot loader executed, and what is the first instruction of the kernel it just loaded?
Answer: The last instruction is "call   *0x10018". The first instruction is "movw   $0x1234,0x472".

3. Where is the first instruction of the kernel?
Answer: The first instruction is "movw   $0x1234,0x472".

4. How does the boot loader decide how many sectors it must read in order to fetch the entire kernel from disk? Where does it find this information?
Answer: Based on ELFHDR->e_phnum read in sector 1.

# Exercise 4

Done

# Exercise 5

The first instruction that would "break" is "ljmp    $PROT_MODE_CSEG, $protcseg" in boot.S, since the wrong link address leads to ljmp fails.

# Exercise 6

The kernel is not loaded into memory when the BIOS enters the boot loader, so it should be some random value.

# Exercise 7
After executing "movl %eax, %cr0", it enables page table mapping so that virtual memory address 0x100000 and 0xf0100000 points to the same physical memory address. If this instruction is commented out, "jmp*%eax" will fail.

# Exercise 8

1. console.c export function cputchar(). printf.c use it for print char to console

2. crt_pos is the cursor position in the screen. When crt_pos goes outside of CRT_SIZE after some changes, we need to update crt_buf to make sure crt_pos is still in the screen.

3. fmt points to the format string, namely "x %d, y %x, z %d\n". ap points to first argument of remaining argument list in stack space. 
The order of exection:
vcprintf
cons_putc('x')
cons_putc(' ')
va_arg before ap points to 1, after ap points to 3
cons_put('1')
cons_put(',')
cons_put(' ')
cons_put('y')
cons_put(' ')
va_arg before ap points to 3, after ap points to 4
cons_put('3')
cons_put(',')
cons_put(' ')
cons_put('z')
cons_put(' ')
va_arg before ap points to 4, after ap points value stored in the previous stack frame
cons_put('4')
cons_put('\n')

4. The output is "He110 World". In vprintffmt, it prints 'H' first. Then it interprets 57616 as hex e110 because of format "%x" and print the number in hex format. Then it prints ' ', 'W', 'o'. Finally, it interprets i as string data because of format "%s" and prints each bytes of 0x00646c72 starting from the least significant byte because of little-endian until it reaches '\0'. 

5. The value stored at the bottom of the previous stack frame would be printed.

6. No need to change cprintf implementation. We could change va_start and va_arg implementation instead by using subtraction to calculate next memory address of argument.

# Exercise 9

The kernel initialize its bootstack in kern/entry.S, its stack is located in [0xf0108000, 0xf0110000]. It allocates space by ".space KSTKSIZE".

# Exercise 10

There are 8 words pushed on the stack, includeing saved %ebp, local variables, call arguments and ret address.

# Exercise 11

Done

# Exercise 12

Done
