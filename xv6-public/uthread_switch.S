	.text

/* Switch from current_thread to next_thread. Make next_thread
 * the current_thread, and set next_thread to 0.
 * Use eax as a temporary register; it is caller saved.
 */
	.globl thread_switch
thread_switch:
	/* YOUR CODE HERE */
	pushal

	movl current_thread, %eax
	movl next_thread, %edx

	movl %edx, current_thread
	movl $0, next_thread

	movl %esp, (%eax)
	movl (%edx), %esp

	popal
	ret				/* pop return address from stack */
