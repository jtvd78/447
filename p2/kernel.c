//
// timeout queue example
//

#include "utils.h"
#include "time.h"
#include "led.h"
#include "uart.h"
#include "errno.h"


#include "os.h"
#include "callout.h"
#include "events.h"




void
notmain()
{
	namenum_t data;
    int timeout;

    // init functions
	init_led();
    init_uart();
    init_timer();
    init_errno();
    init_timeoutq();

	blink_led_stall(10);

    // wake up
    uart_puts("\n");
    uart_puts("\n");
	log("");
    log("System is booting.");
    log("Please hit enter to continue.\n");

    uart_recv();

	debug(DEBUG_LOW, "Test DEBUG_LOW", 0);

	void * ptr = GETPC();
	uart_put64x(sizeof(ptr)); uart_puts("\n");
	uart_put64x((unsigned long long)ptr); uart_puts("\n");

    // create some timeout events
	data.num = 3;
	bring_timeoutq_current();
    create_timeoutq_event( 2 * ONE_SEC, 4 * ONE_SEC, do_hex, data );
	data.num = 20;
	bring_timeoutq_current();
    create_timeoutq_event( 3 * ONE_SEC, 4 * ONE_SEC, do_blink, data );
	data.num = 0xabcde0123456789;
	bring_timeoutq_current();
    create_timeoutq_event( 4 * ONE_SEC, 4 * ONE_SEC, do_hex, data );
	data.name[0] = 'H';
	data.name[1] = 'e';
	data.name[2] = 'l';
	data.name[3] = 'l';
	data.name[4] = 'o';
	data.name[5] = '.';
	data.name[6] = 0;
	data.name[7] = 0;
	bring_timeoutq_current();
    create_timeoutq_event( 5 * ONE_SEC, 4 * ONE_SEC, do_string, data);

    uart_puts("TQ items created; running loop.\n");
    uart_puts("\n");

    // run the list
//	extern pfv_t tq_gofunc();
    while (1) {
//		debug(DEBUG_LOW, "top of loop eventpc = ", (unsigned long)tq_gofunc());
		debug(DEBUG_LOW, "top of loop event", 0);
		if (handle_timeoutq_event()) {
			debug(DEBUG_LOW, "handled event.", 0);
			continue;
		}
		timeout = bring_timeoutq_current();
		debug(DEBUG_LOW, "no event. about to wait for = ", (unsigned long)timeout);
		if (DEBUG_LEVEL >= DEBUG_LOW) {
			data.num = (unsigned long)get_time();	// since we're not using data anymore
		}
		wait(timeout);
		debug(DEBUG_LOW, "done waiting = ", get_time() - (time_t)data.num);
    }

	return;

}
