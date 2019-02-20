
typedef unsigned long time_t;

extern time_t get_time ( void );
extern void wait( time_t usecs );
extern void init_timer();

#define ONE_USEC	0x1
#define ONE_MSEC	(ONE_USEC * 1000)
#define ONE_SEC		(ONE_MSEC * 1000)

#define MSEC(x)		(x * ONE_MSEC)
