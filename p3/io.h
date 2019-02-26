
//
// IO.h
//

struct dev {
	char devname[8];
	int	devtype;
	pfv_t init;
	pfi_t read;
	pfi_t write;
};

#define	CBUFSIZE	128
#define	NUM_CIOBUFS	64

typedef char cbuffer[CBUFSIZE];

enum devices {
	DEV_NULL,
	DEV_LED,
	DEV_CONSOLE,
	DEV_CLOCK,
	DEV_KLOG,
	// insert new ones here
	MAX_DEVICES,
};

enum dev_types {
	DEV_INVALID,
	DEV_WORD,
	DEV_STREAM,
	DEV_BLOCK,
};


extern void init_io();
