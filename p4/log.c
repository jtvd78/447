
#include "os.h"


char *errmsg[ERRNO_MAX+1] =
{
	"NO ERROR: ",
	"Error: FAILED ASSERTION - ",
	"Error: OUT OF MEMORY - ",
	"Error: UNKNOWN INTERRUPT - ",
	"Error: UNSUPPORTED SYSTEM CALL - ",
	"Error: BAD CALL TO DEVHANDLER - ",

	/* add new ones above this line */
	"Error: ERRNO INPUT EXCEEDS MAXIMUM - ",
};


void
init_log()
{
	// nothing for now
}


#define MSEC_PER_USEC	1000
#define SEC_PER_USEC	(1000 * MSEC_PER_USEC)
#define MIN_PER_USEC	(60 * SEC_PER_USEC)

void
print_time()
{
	uint64_t time = get_time();
	unsigned long now = (long)(time & 0xFFFFFFFF);

	uart_put2d((now / MIN_PER_USEC) % 60);		// min mod 60
	uart_puts(":");
	uart_put2d((now / SEC_PER_USEC) % 60);		// sec mod 60
	uart_puts(".");
	uart_put3d((now / MSEC_PER_USEC) % 1000);		// msec mod 1000
}

void
log(char *msg, long num)
{
	switch (cpu_id()) {
		case 0: 
			uart_puts("[c0|");
			break;
		case 1:
			uart_puts("[c1|");
			break;
		case 2:
			uart_puts("[c2|");
			break;
		case 3:
			uart_puts("[c3|");
			break;
		default:
			panic(ERRNO_ASSERT, "unkown core in console log function");
			break;
	}
	print_time();
	uart_puts("] ");

	if (msg != (char *)NULL) {
		uart_puts(msg);
	}
	if (num != NOVAL) {
		uart_put32x(num);
	}
	uart_puts("\n");
}

void
dont_call_directly(void * pc, char *msg, unsigned long data)
{
	print_time();
	uart_puts("<pc="); uart_put32x((unsigned long)pc); uart_puts("> - ");
	uart_puts(msg); uart_put32x(data); uart_puts("\n");
}

void
panic(int num, char *location)
{
	if (num < 0 || num >= ERRNO_MAX) {
		num = ERRNO_MAX;
	}
	uart_puts(errmsg[num]);
	uart_puts(location);
	uart_puts("\n");

	while (1)
	;
}

void
die(long pc)
{
	log("kernel panic in assembly code, pc = ", pc);

	while (1)
	;
}

