
#include "os.h"
#include "dev.h"


int
io_error(long arg)
{
	log("Bad R/W handler in io_error, arg - ", arg);
	panic(ERRNO_DEVHANDLER, "io_error from devtab[]");
	// does not return;
	return 0;
}

int
io_write_led(int buf)
{
	// your code goes here
}

int
io_get_time(uint64_t *buf, int size)
{
	// your code goes here
}

int
io_uart_recv()
{
	// your code goes here
}

int
io_uart_send(unsigned int buf)
{
	// your code goes here
}

int
io_klog(char *buf, int siz)
{
	// your code goes here
}


//
// struct dev {
//	char name[8];
//	int type;
//	pfv_t init();
//	pfi_t read();
//	pfi_t write();
// }
//
struct dev devtab[MAX_DEVICES+1] = {
	{
		"Null",
		DEV_INVALID,
		dummy,
		(pfi_t)dummy,
		(pfi_t)dummy,
	},
	{
		"LED",
		DEV_WORD,
		init_led,
		io_error,
		io_write_led,
	},
	{
		"Console",
		DEV_WORD,
		init_uart,
		io_uart_recv,
		io_uart_send,
	},
	{
		"Clock",
		DEV_STREAM,
		init_time,
		io_get_time,
		io_error,
	},
	{
		"KernLog",
		DEV_STREAM,
		init_log,
		io_error,
		io_klog,
	},
	{
		"NONE",
		DEV_INVALID,
		(pfv_t)io_error,
		io_error,
		io_error,
	},
};


void
init_io(char *kver)
{
	struct dev *dp;
	long i;

	for (dp = devtab, i=0; i<MAX_DEVICES; i++, dp++) {
		dp->init();
	}
}



