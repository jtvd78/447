
//
// dev.h
//

#define	CBUFSIZE	128

typedef char cbuffer_t[CBUFSIZE];

enum devices {
	DEV_NULL,
	DEV_LED,
	DEV_CONSOLE,
	DEV_CLOCK,
	DEV_KLOG,
	// insert new ones here
	MAX_DEVICES,
};
