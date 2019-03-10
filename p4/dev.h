
//
// dev.h
//

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

