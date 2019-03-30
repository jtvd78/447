
#include "os.h"
#include "syscall.h"

extern struct dev devtab[];
extern int strcpyN(char *to, char *from, int bufsiz);
extern	long runningthreadid;

int
trap_handler(unsigned long r0, unsigned long r1, unsigned long r2)
{
	register long r7 asm ("r7");

	long lr, sp, spsr;
	namenum_t data;

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
	debug(DEBUG_LOW, "   r7", r7);
	debug(DEBUG_LOW, "   r0", r0);
	debug(DEBUG_LOW, "   r1", r1);
	debug(DEBUG_LOW, "   r2", r2);
	debug(DEBUG_LOW, "   SP", sp);
	debug(DEBUG_LOW, "  EPC", lr);
	debug(DEBUG_LOW, " SPSR", spsr);

	switch (r7) {
		// for non-error conditions, return from within the switch() statement
		case SYSCALL_KVERSION:
			debug(DEBUG_LOW, "SYSCALL_WR_WORD dev  =", r0);
			return strcpyN((char *)r0, kversion, CBUFSIZE);
			break;
		case SYSCALL_RD_WORD:
			debug(DEBUG_HI, "SYSCALL_RD_WORD dev =", r0);
			if (r0 < MAX_DEVICES) {

				// your code goes here

			}
			break;
		case SYSCALL_WR_WORD:
			debug(DEBUG_LOW, "SYSCALL_WR_WORD dev  =", r0);
			debug(DEBUG_LOW, "SYSCALL_WR_WORD data =", r1);
			if (r0 < MAX_DEVICES) {
				if (devtab[r0].wcheck()) {
					return devtab[r0].write(r1);
				}
				
				// your code goes here

			}
			break;
		case SYSCALL_RD_CBUF:
			debug(DEBUG_LOW, "SYSCALL_RD_CBUF dev =", r0);
			debug(DEBUG_LOW, "SYSCALL_RD_CBUF buf =", r1);
			debug(DEBUG_LOW, "SYSCALL_RD_CBUF siz =", r2);
			if (r0 < MAX_DEVICES) {
				return devtab[r0].read(r1, r2);
			}
			break;
		case SYSCALL_WR_CBUF:
			debug(DEBUG_LOW, "SYSCALL_WR_CBUF dev =", r0);
			debug(DEBUG_LOW, "SYSCALL_WR_CBUF buf =", r1);
			debug(DEBUG_LOW, "SYSCALL_RD_CBUF siz =", r2);
			if (r0 < MAX_DEVICES) {
				return devtab[r0].write(r1, r2);
			}
			break;
		case SYSCALL_START_THREAD:
			// or just do this for test purposes:
			// create_thread("blinker", do_blinker);
			// return 0;
			debug(DEBUG_LOW, "SYSCALL_START THREAD name =", r0);
			debug(DEBUG_LOW, "SYSCALL_START THREAD addr =", r1);
			data.num = r0;
			log(data.name, r1);
			create_thread(data.name, (void *)r1);
			return 0;
			break;
		case SYSCALL_PROCESSES:
			dump_processes();
			return 0;
			break;
        case SYSCALL_DUMP_RANGE:
            log("SYSCALL_DUMP_RANGE start =", r0);
            log("SYSCALL_DUMP_RANGE end   =", r1);
            log("SYSCALL_DUMP_RANGE wide  =", r2);
            r0 = r0 & 0xFFFFFFFC;   // zeroes out the bottom two bits
            while (r0 < r1) {
                switch (r2) {
                    case 8:
                        uart_put32x(r0); uart_puts(": "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        uart_puts("\n");
                        r0+=4;
                        break;
                    case 4:
                        uart_put32x(r0); uart_puts(": "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        uart_puts("\n");
                        r0+=4;
                        break;
                    case 2:
                        uart_put32x(r0); uart_puts(": "); uart_put32x(GET32(r0));
                        r0+=4; uart_puts(" "); uart_put32x(GET32(r0));
                        uart_puts("\n");
                        r0+=4;
                        break;
                    default:
                        uart_put32x(r0); uart_puts(": "); uart_put32x(GET32(r0)); uart_puts("\n");
                        r0+=4;
                        break;
                }
            }
            return 0;
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
