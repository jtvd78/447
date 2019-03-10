//
// callout table - timeout queue
//

#include "os.h"


#define MAX_EVENTS	256
struct event queue[ MAX_EVENTS ];

// list anchors -- important, but ignore them; they are never used directly
llobject_t TQ;
llobject_t FL;

struct event *timeoutq;
struct event *freelist;

uint64_t then_usec;

//
// sets up concept of local time
// initializes the timeoutq and freelist
// fills the freelist with entries
// timeoutq is empty
//
void
init_timeoutq()
{
	int i;

	timeoutq = (struct event *)&TQ;
	LL_INIT(timeoutq);
	freelist = (struct event *)&FL;
	LL_INIT(freelist);

	for (i=0; i<MAX_EVENTS; i++) {
		struct event *ep = &queue[i];
		LL_PUSH(freelist, ep);
	}

	then_usec = get_time();

	return;
}


//
// account for however much time has elapsed since last update
// return timeout or MAX
//
// note: then_usec matches the most recent update to the TQ head entry
//
int
bring_timeoutq_current()
{
	struct event *ep;
	uint64_t delta;
	uint64_t now = get_time();

	ep = (struct event *)LL_TOP(timeoutq);
	if (ep) {
		delta = now - then_usec;
		ep->timeout -= (int)(delta & 0x7FFFFFFF);
	}

	then_usec = now;

	if (ep) {
		return ep->timeout;
	} else {
		return MAX_SLEEP_INTERVAL;
	}
}


void
insert_event(struct event *ep)
{
	struct event *tq;
	LL_EACH(timeoutq, tq, struct event) {
		if (tq->timeout > ep->timeout) {
			tq->timeout -= ep->timeout;
			LL_L_INSERT(tq, ep);
			return;
		}
		ep->timeout -= tq->timeout;
	}

	LL_APPEND(timeoutq, ep);
	return;
}


//
// get a new event structure from the freelist,
// initialize it, and insert it into the timeoutq
// 
void
create_timeoutq_event(int timeout, int repeat, pfv_t function, namenum_t data)
{
	struct event *ep;

	ep = (struct event *)LL_POP(freelist);
	if (ep == (struct event *)NULL) {
		panic(ERRNO_SPACE, "create TQ event");
		return;
	}

	ep->timeout = timeout;
	ep->repeat_interval = repeat;
	ep->go = function;
	ep->data = data;

	insert_event(ep);

	return;
}



//
// bring the time current
// check the list to see if anything expired
// if so, do it - and either return it to the freelist 
// or put it back into the timeoutq
//
// return whether or not you handled anything
//
int
handle_timeoutq_event( )
{
	struct event *ep, *next;

	bring_timeoutq_current();

	LL_EACH(timeoutq, ep, struct event) {
		if (ep->timeout <= MIN_TIMEOUT) {
			break;
		} else {
			return 0;
		}
	}

	if (ep == timeoutq) {
		return 0;
	}

	next = (struct event *)ep->next;
	if (next != timeoutq) {
		next->timeout += ep->timeout;
	}
	ep = (struct event *)LL_DETACH(timeoutq, ep);

#if DEBUG_LEVEL < DEBUG_NONE
	uint64_t debug_time = 0;

	debug(DEBUG_MED, "about to handle event at ", (unsigned long)(ep->go));
	debug(DEBUG_MED, "time since last callout event ", get_time() - debug_time);
	debug_time = get_time();
#endif

	ep->go(ep);

	if (ep->repeat_interval > 0 && --ep->max_repeats > 0) {
		ep->timeout = ep->repeat_interval;
		insert_event(ep);
	} else {
		LL_PUSH(freelist, ep);
	}

	return 1;
}

// for debugging
pfv_t
tq_gofunc()
{
    struct event *ep;
    ep = (struct event *)LL_TOP(timeoutq);
    if (!ep) {
        panic(ERRNO_ASSERT, "tq_gofunc returning NULL pointer? ...");
    }
    return ep->go;
}


