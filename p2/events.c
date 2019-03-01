#include "os.h"
#include "uart.h"
#include "led.h"
#include "time.h"
#include "callout.h"

void do_hex( namenum_t data )
{
	uart_put64x(data.num); uart_puts("\n");
}

void do_blink( namenum_t data )
{
#if 0	// change this to 0 to try the more complex approach
	blink_led_stall(data.num);
#else
	int i;
	namenum_t foo;
	foo.num = 0;
	bring_timeoutq_current();
	for (i=0; i<(data.num*2); i++) {
		if (i & 0x1) {
			create_timeoutq_event( 50 * i * ONE_MSEC, 0, led_off, foo );
		} else {
			create_timeoutq_event( 50 * i * ONE_MSEC, 0, led_on, foo );
		}
	}
#endif
}

void do_butter( namenum_t data )
{
	long time=0;

	bring_timeoutq_current();
	create_timeoutq_event( time, 0, led_on, (namenum_t)time );

	time = 750 * ONE_MSEC;
	create_timeoutq_event( time, 0, led_on, (namenum_t)time );
	create_timeoutq_event( time - MSEC(100), 0, led_off, (namenum_t)time );

	time = 1000 * ONE_MSEC;
	create_timeoutq_event( time, 0, led_on, (namenum_t)time );
	create_timeoutq_event( time - MSEC(100), 0, led_off, (namenum_t)time );

	time = 1500 * ONE_MSEC;
	create_timeoutq_event( time, 0, led_on, (namenum_t)time );
	create_timeoutq_event( time - MSEC(100), 0, led_off, (namenum_t)time );

	time = 2000 * ONE_MSEC;
	create_timeoutq_event( time, 0, led_on, (namenum_t)time );
	create_timeoutq_event( time - MSEC(100), 0, led_off, (namenum_t)time );

	time = 2250 * ONE_MSEC;
	create_timeoutq_event( time, 0, led_on, (namenum_t)time );
	create_timeoutq_event( time - MSEC(100), 0, led_off, (namenum_t)time );

	create_timeoutq_event( time + MSEC(200), 0, led_off, (namenum_t)time );
}


void do_string( namenum_t data )
{
	uart_puts(data.name); uart_puts("\n");
}
