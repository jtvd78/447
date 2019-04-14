//
// timeout queue example
//

#include "os.h"

extern struct dev devtab[];
extern void run_shell();

int count;

void
periodic_timer()
{
//	if ((count++ % 1000) == 0) log("periodic timer called", NOVAL);

	uint64_t time = get_time();
	time += MAX_BLOCK_TIME;

	do {
		if (!handle_timeoutq_event()) break;
	} while (get_time() <= time);

	scheduler(THREAD_RUN);

	return;
}

void
init_kernel()
{
	struct dev *dp;
	int i;

    // init functions
    init_io();

    // init timeoutq
    init_timeoutq();

	// I/O devices are up and running.
	blink_led_stall(5);

	// initial console output
    uart_puts("\n\n");
	log("...", NOVAL);
    log("System is booting, kernel cpuid =", cpu_id());
	log(kversion, NOVAL);

	if (DEBUG_MED >= DEBUG_LEVEL) {
		log("Available devices:", NOVAL);
		for (dp = devtab, i=0; i<MAX_DEVICES; i++, dp++) {
			log(dp->devname, NOVAL);
			log("Device number: ", i);
			log("Device type:   ", dp->devtype);
			log("Init function: ", (long)(dp->init));
			log("Read function: ", (long)(dp->read));
			log("Write function:", (long)(dp->write));
			log("Rck function:  ", (long)(dp->rcheck));
			log("Wck function:  ", (long)(dp->wcheck));
		}
	}

	init_threads();
	create_thread("shell", run_shell);

    // wake up
	log("...", NOVAL);
    log("Init complete. Please hit any key to continue.", NOVAL);

    uart_recv();

	scheduler(THREAD_INIT);
}

