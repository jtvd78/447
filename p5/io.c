
#include "os.h"

#define MAX_IOS		32
#define MAX_CBUFS	16
#define MAX_BLOCKS	8

struct io io_array[ MAX_IOS ];

llobject_t IOF;
struct io *io_free;


struct cbuf {
	LL_PTRS;
	int ct;
	char *ptr;
	cbuffer_t buffer;
} cbufs[MAX_CBUFS];

llobject_t CF;
struct cbuf *cfree;

struct bbuf {
	LL_PTRS;
	int ct;
	char *ptr;
	char block[1024 * 4];
} bbufs[MAX_BLOCKS];

llobject_t BF;
struct bbuf *bfree;


struct io *
get_io_entry(int type)
{
	struct io *iop;
	struct bbuf *bp;
	struct cbuf *cp;

	switch (type) {
		case DEV_WORD:
		case DEV_CBUF:
		case DEV_BLOCK:
			iop = (struct io *)LL_POP(io_free);
			if (iop == (struct io *)NULL) {
				log("ERROR: out of space in get_io_entry: io", NOVAL);
				return (struct io *)NULL;
			}
			iop->type = type;
			break;
		default:
			log("Bad devtype in get_io_entry", type);
			return (struct io *)NULL;
	}

	switch (type) {
		case DEV_WORD:
			break;
		case DEV_CBUF:
			cp = (struct cbuf *)LL_POP(cfree);
			if (cp == (struct cbuf *)NULL) {
				log("ERROR: out of space in get_io_entry: cbuf", NOVAL);
				return (struct io *)NULL;
			}
			iop->data.ptr = (void *)cp;
			break;
		case DEV_BLOCK:
			bp = (struct bbuf *)LL_POP(bfree);
			if (bp == (struct bbuf *)NULL) {
				log("ERROR: out of space in get_io_entry: bbuf", NOVAL);
				return (struct io *)NULL;
			}
			iop->data.ptr = (void *)bp;
			break;
		default:
			// absolutely should not get here
			log("Weirdness in get_io_entry, type =", type);
			return (struct io *)NULL;
	}
	return iop;
}

void
free_io_entry(struct io *iop)
{
	if (!iop) {
		log("Memory-Leak Warning: NULL pointer in free_io_entry", NOVAL);
		return;
	}

	switch (iop->type) {
		case DEV_WORD:
			break;
		case DEV_CBUF:
			LL_PUSH(cfree, (struct cbuf *)(iop->data.ptr));
			break;
		case DEV_BLOCK:
			LL_PUSH(bfree, (struct bbuf *)(iop->data.ptr));
			break;
		default:
			log("Memory-Leak Warning: Bad type in free_io_entry", iop->type);
			return;
	}

	LL_PUSH(io_free, iop);
	return;
}


int
io_error(long arg)
{
	log("Bad R/W handler in io_error, arg -", arg);
	panic(ERRNO_DEVHANDLER, "io_error from devtab[]");
	// does not return;
	return 0;
}

int
io_write_led(int buf)
{
	if (buf) {
		led_on();
	} else {
		led_off();
	}
	return 1;
}

int
io_get_time(uint64_t *buf, int size)
{
	if (size < 8) return 0;

	uint64_t time = get_time();
	*buf = time;

	return 1;
}

int
io_uart_recv()
{
	return uart_recv();
}

int
io_uart_send(unsigned int data)
{
	uart_send(data);
	return 1;
}

int
io_klog(char *buf, int siz)
{
	log(buf, NOVAL);
	return 1;
}

int
io_valid()
{
	return 1;
}


//
// struct dev {
//	char name[8];
//	int type;
//	pfv_t init();
//	pfi_t read();
//	pfi_t write();
//	pfi_t rcheck();
//	pfi_t wcheck();
// }
//
struct dev devtab[MAX_DEVICES+1] = {
	{
		"Null",
		DEV_INVALID,
		dummy,
		(pfi_t)dummy,
		(pfi_t)dummy,
		(pfi_t)dummy,
		(pfi_t)dummy,
	},
	{
		"LED",
		DEV_WORD,
		init_led,
		io_error,
		io_write_led,
		io_error,
		io_valid,
	},
	{
		"Console",
		DEV_WORD,
		init_uart,
		io_uart_recv,
		io_uart_send,
		uart_recvcheck,
		uart_sendcheck,
	},
	{
		"Clock",
		DEV_CBUF,
		init_time,
		io_get_time,
		io_error,
		io_valid,
		io_error,
	},
	{
		"KernLog",
		DEV_CBUF,
		init_log,
		io_error,
		io_klog,
		io_error,
		io_valid,
	},
	{
		"NONE",
		DEV_INVALID,
		(pfv_t)io_error,
		io_error,
		io_error,
		io_error,
		io_error,
	},
};


void
init_io()
{
	struct dev *dp;
	long i;

	for (dp = devtab, i=0; i<MAX_DEVICES; i++, dp++) {
		dp->init();
	}

	io_free = (struct io *)&IOF;
	LL_INIT(io_free);

	for (i=0; i<MAX_IOS; i++) {
		struct io *iop = &io_array[i];
		LL_PUSH(io_free, iop);
	}

	cfree = (struct cbuf *)&CF;
	LL_INIT(cfree);

	for (i=0; i<MAX_CBUFS; i++) {
		struct cbuf *cbp = &cbufs[i];
		LL_PUSH(cfree, cbp);
	}

	bfree = (struct bbuf *)&BF;
	LL_INIT(bfree);

	for (i=0; i<MAX_BLOCKS; i++) {
		struct bbuf *bbp = &bbufs[i];
		LL_PUSH(bfree, bbp);
	}

}


void
do_dev_word(struct event *ep)
{
	struct io *iop = (struct io *)(ep->data.ptr);
	int ch;

	if (!iop) panic(ERRNO_ASSERT, "do_dev_word");

	debug(DEBUG_MED, "do_dev_word tid", iop->threadid);
	debug(DEBUG_MED, "do_dev_word  rw", iop->rw);
	debug(DEBUG_MED, "do_dev_word dev", iop->device);
	debug(DEBUG_MED, "do_dev_word num", iop->data.num);
	debug(DEBUG_MED, devtab[iop->device].devname, iop->data.num);

	// your code goes here

}

