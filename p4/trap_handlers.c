
#include "os.h"
#include "syscall.h"
#include "dev.h"

extern struct dev devtab[];

extern int strcpyN(char *to, char *from, int bufsiz);

int
trap_handler(unsigned long r0, unsigned long r1, unsigned long r2)
{
	register long r7 asm ("r7");

	long lr, sp, spsr;

	asm volatile ("mov %[lr], lr\n"
					: [lr] "=r" (lr)
					:
					: );

	asm volatile ("mov %[sp], sp\n"
					: [sp] "=r" (sp)
					:
					: );

	asm volatile ("mrs %[spsr], spsr\n"
					: [spsr] "=r" (spsr)
					:
					: );

	debug(DEBUG_LOW, "Trap Handler:", NOVAL);
	debug(DEBUG_LOW, "   r7 ", r7);
	debug(DEBUG_LOW, "   r0 ", r0);
	debug(DEBUG_LOW, "   r1 ", r1);
	debug(DEBUG_LOW, "   r2 ", r2);
	debug(DEBUG_LOW, "   SP ", sp);
	debug(DEBUG_LOW, "  EPC ", lr);
	debug(DEBUG_LOW, " SPSR ", spsr);

	switch (r7) {
		// for non-error conditions, return from within the switch() statement
		case SYSCALL_KVERSION:
			debug(DEBUG_LOW, "SYSCALL_WR_WORD dev  = ", r0);

			// your code goes here

			break;
		case SYSCALL_RD_WORD:
			debug(DEBUG_LOW, "SYSCALL_RD_WORD dev = ", r0);

			// your code goes here

			break;
		case SYSCALL_WR_WORD:
			debug(DEBUG_LOW, "SYSCALL_WR_WORD dev  = ", r0);
			debug(DEBUG_LOW, "SYSCALL_WR_WORD data = ", r1);

			// your code goes here

			break;
		case SYSCALL_RD_STREAM:
			debug(DEBUG_LOW, "SYSCALL_RD_STREAM dev = ", r0);
			debug(DEBUG_LOW, "SYSCALL_RD_STREAM buf = ", r1);
			debug(DEBUG_LOW, "SYSCALL_RD_STREAM siz = ", r2);

			// your code goes here

			break;
		case SYSCALL_WR_STREAM:
			debug(DEBUG_LOW, "SYSCALL_WR_STREAM dev = ", r0);
			debug(DEBUG_LOW, "SYSCALL_WR_STREAM buf = ", r1);
			debug(DEBUG_LOW, "SYSCALL_RD_STREAM siz = ", r2);

			// your code goes here

			break;

		// these are all error conditions:
		case SYSCALL_INVALID:
			debug(DEBUG_HI, "SYSCALL_INVALID", NOVAL);
			break;
		case MAX_SYSCALLS:
			debug(DEBUG_HI, "MAX_SYSCALLS", NOVAL);
			break;
		default:
			break;
	}

	// if we get here, it is an error
	panic(ERRNO_SYSCALL, "trap_handler");
	// for gcc
	return 0;
}
