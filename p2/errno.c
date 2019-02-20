
#include "utils.h"
#include "errno.h"
#include "time.h"
#include "uart.h"
#include "os.h"

char *errmsg[ERRNO_MAX+1];


void
init_errno()
{
	errmsg[ ERRNO_NONE ] = "NO ERROR: ";
	errmsg[ ERRNO_ASSERT ] = "Error: FAILED ASSERTION - ";
	errmsg[ ERRNO_SPACE ] = "Error: OUT OF MEMORY - ";
	errmsg[ ERRNO_NO_FREE_EVENT] = "Error: NO FREE EVENT - ";
	errmsg[ ERRNO_NO_QUEUE_HEAD] = "Error: NO QUEUE HEAD - ";


	/* add new ones above this line */
	errmsg[ ERRNO_MAX ] = "Error: ERRNO INPUT EXCEEDS MAXIMUM - ";
}

#define MSEC_PER_USEC	1000
#define SEC_PER_USEC	(1000 * MSEC_PER_USEC)
#define MIN_PER_USEC	(60 * SEC_PER_USEC)
#define HRS_PER_USEC	(60 * SEC_PER_USEC)
void
print_time()
{
	time_t now = get_time();

	uart_put2d(now / HRS_PER_USEC);				// hrs mod 100
	uart_puts(":");
	uart_put2d((now / MIN_PER_USEC) % 60);		// min mod 60
	uart_puts(":");
	uart_put2d((now / SEC_PER_USEC) % 60);		// sec mod 60
	uart_puts(".");
	uart_put3d((now / MSEC_PER_USEC) % 1000);	// msec mod 1000
}

void
hex_print_time()
{
	time_t now = get_time();

	uart_put8x((unsigned int)(now >> 32));				// hrs mod 256
	uart_puts(":");
	uart_put8x((unsigned int)((now >> 26) & 0x3f));		// min mod 64
	uart_puts(":");
	uart_put8x((unsigned int)((now >> 20) & 0x3f));		// sec mod 64
	uart_puts(".");
	uart_put12x((unsigned int)((now >> 10) & 0x3ff));	// msec mod 1024

// it takes ~1ms just to run this code, so don't both printing out usec
//	uart_puts(".");
//	uart_put12x((unsigned int)(now & 0x3ff));			// usec mod 1024

}

void
log(char *msg)
{
	switch (get_core()) {
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
	uart_puts("] "); uart_puts(msg); uart_puts("\n");
}

void
dont_call_directly(void * pc, char *msg, unsigned long data)
{
	print_time();
	uart_puts("<pc="); uart_put64x((unsigned long)pc); uart_puts("> - ");
	uart_puts(msg); uart_put64x(data); uart_puts("\n");
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

