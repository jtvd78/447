//
// timeout queue example
//

#include "os.h"

#include <stdbool.h>			// Neede for bool
#include <stdint.h>				// Needed for uint32_t, uint16_t etc
#include "SDCard.h"

extern struct dev devtab[];
extern char *returns[];

int count;

void
periodic_timer()
{
	if (DEBUG_LOW >= DEBUG_LEVEL && (count++ % 100) == 0) debug(DEBUG_LOW, "periodic timer called", NOVAL);

	uint64_t time = get_time();
	time += MAX_BLOCK_TIME;

	do {
		if (!handle_timeoutq_event()) break;
	} while (get_time() <= time);

	scheduler(THREAD_RUN);

	return;
}


char buf[1024];

void
test_read()
{
	uint32_t bytesRead;
	char *filename = "kernel7.img";

	log("test_read - SD Card example usage", NOVAL);

	HANDLE fHandle = sdCreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (fHandle != 0 && fHandle <= 8) {
		log("Reading file into buf at", (long)buf);
		log(filename, NOVAL);
		if (sdReadFile(fHandle, (void *)buf, 1024, &bytesRead, 0) == true) {
			uart_puts("\n");
			idump((int *)buf);
			uart_puts("\n");
			log("Compare output to first 256 bytes of kernel7.list", NOVAL);
		} 
		sdCloseHandle(fHandle);
	}

	log("Done.", NOVAL);
	while (1)
		;
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

    // init SD card
    log("Initializing SD Card ...", NOVAL);
    log("---------------> sdInitCard [init]", NOVAL);
	while ((i = sdInitCard(NULL, NULL, true)) != SD_OK) {
		log(returns[i], i);
	}
    log("---------------> sdInitCard [term]", NOVAL);
    log("SD Card working.", NOVAL);

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
	create_thread("Eggshell", "shell.bin", 0x00000000);	// need to decide where to put it (don't use 0x00000000)

    // wake up
	log("...", NOVAL);
    log("Init complete. Please hit any key to continue.", NOVAL);

	// wait on user input
    uart_recv();


	//----
	test_read();	// this does not return
	//----


	// start up the default task
	scheduler(THREAD_INIT);
}

