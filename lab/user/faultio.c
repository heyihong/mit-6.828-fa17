// test user-level fault handler -- alloc pages to fix faults

#include <inc/lib.h>
#include <inc/x86.h>

void
umain(int argc, char **argv)
{
        int x, r;
	int nsecs = 1;
	int secno = 0;
	int diskno = 1;

	if (read_eflags() & FL_IOPL_3)
		cprintf("eflags wrong\n");
	
	// this outb to select disk 1 should result in a general protection
	// fault, because user-level code shouldn't be able to use the io space.
	// Changed test port from 0x1F6 to 0x8A00, since 0x1F6 seems to be allowed
	// even when IOPL of eflags is 0.
	outb(0x8A00, 0xE0 | (1<<4));

        cprintf("%s: made it here --- bug\n");
}
