
extern unsigned long long now_usec ( void );
extern void wait( unsigned int usecs );
extern void init_timer();

#define ONE_USEC	0x1
#define ONE_MSEC	(ONE_USEC << 10)
#define ONE_SECOND	(ONE_MSEC << 10)

