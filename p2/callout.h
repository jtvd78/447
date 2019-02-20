
#include "llist.h"

struct event {
	LL_PTRS;
	int timeout;
	int repeat_interval;
	pfv_t go;
	namenum_t data;
};

extern void init_timeoutq( void );
extern int bring_timeoutq_current( void );
extern void create_timeoutq_event( int, int, pfv_t, namenum_t );
extern int handle_timeoutq_event( void );

#define MIN_TIMEOUT ONE_MSEC
