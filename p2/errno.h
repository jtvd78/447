
enum debug_levels {
	DEBUG_LOW,
	DEBUG_MED,
	DEBUG_HI,
	DEBUG_NONE
};

//
// set this to the level you want to print out --
// that level and higher will print to screen
//
#define DEBUG_LEVEL DEBUG_NONE

extern void	dont_call_directly( void *, char *, unsigned long);
#if (DEBUGLEVEL == DEBUG_NONE)
#define debug(x,y,z);	
#else
#define debug(x,y,z);	if (x >= DEBUG_LEVEL) { dont_call_directly(GETPC(),y,z); }
#endif
extern void	log( char *);
extern void	panic( int, char *);
extern void init_errno( void );

enum err_numbers {
	ERRNO_NONE,
	ERRNO_ASSERT,
	ERRNO_SPACE,
	ERRNO_NO_FREE_EVENT,
	ERRNO_NO_QUEUE_HEAD,
	/* add new ones above this line */
	ERRNO_MAX
};

