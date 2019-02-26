
enum debug_levels {
	DEBUG_LOW,
	DEBUG_MED,
	DEBUG_HI,
	DEBUG_NONE
};

#define DEBUG_LEVEL	DEBUG_MED

extern void * GETPC();
#if 0
#define debug(x,y,z);
#else
#define debug(x,y,z);	if (x >= DEBUG_LEVEL) { dont_call_directly(GETPC(),y,z); }
#endif

extern void	dont_call_directly( void *, char *, unsigned long);
extern void	log( char *, long);
extern void	panic( int, char *);
extern void init_log( void );
extern void die( long pc );

enum err_numbers {
	ERRNO_NONE,
	ERRNO_ASSERT,
	ERRNO_SPACE,
	ERRNO_INTERRUPT,
	ERRNO_SYSCALL,
	ERRNO_DEVHANDLER,
	/* add new ones above this line */
	ERRNO_MAX
};

#define NOVAL	0xbadf000d
