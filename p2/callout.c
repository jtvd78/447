//
// callout table - timeout queue
//

#include "time.h"
#include "errno.h"
#include "os.h"

#include "callout.h"



#define MAX_EVENTS	256
struct event queue[ MAX_EVENTS ];

// list anchors -- important, but ignore them; they are never used directly
llobject_t TQ;
llobject_t FL;

struct event *timeoutq;
struct event *freelist;

time_t then_usec;

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
int
bring_timeoutq_current()
{

	debug(DEBUG_LOW, "Begin bring_timeoutq_current", 0);

	struct event* first = (struct event*) LL_FIRST(timeoutq);
	time_t now = get_time();
	time_t diff = now - then_usec;

	debug(DEBUG_LOW, "bring_timeoutq_current. Diff=", diff);

	if (first != NULL) {

		debug(DEBUG_LOW, "bring_timeoutq_current. Old Timeout=", first->timeout);

		first->timeout -= diff;

		debug(DEBUG_LOW, "bring_timeoutq_current. New Timeout=", first->timeout);

	}

	then_usec = now;

	return first->timeout;
}

//
// get a new event structure from the freelist,
// initialize it, and insert it into the timeoutq
// 
void
create_timeoutq_event(int timeout, int repeat, pfv_t function, namenum_t data)
{
	
	debug(DEBUG_LOW, "Begin create_timeoutq_event", 0);

	debug(DEBUG_LOW, "create_timeoutq_event. Event timeout=", timeout);
	
	struct event* first_free = (struct event*) LL_POP(freelist);
	if (first_free == NULL) {
		panic(ERRNO_NO_FREE_EVENT, "create_timeoutq_event");
	}

	first_free->repeat_interval = repeat;
	first_free->go = function;
	first_free->data = data;	

	int added_event = 0;
	int timeout_left = timeout;
	struct event* tmp;

	debug(DEBUG_LOW, "create_timeoutq_event event loop", 0);

	int element_ctr = 0;

	LL_EACH (timeoutq, tmp, struct event) {
		timeout_left -= tmp->timeout;

		debug(DEBUG_LOW, "create_timeoutq_event. element num=", element_ctr);
		debug(DEBUG_LOW, "create_timeoutq_event. timeout_left=", timeout_left);
		debug(DEBUG_LOW, "create_timeoutq_event. tmp->timeout=", tmp->timeout);

		if(timeout_left < 0) {
			// Insert new event before tmp
			LL_L_INSERT(tmp, first_free);
			first_free->timeout = tmp->timeout + timeout_left;
			tmp->timeout -= first_free->timeout;
			added_event = 1;
			break;
		}

		element_ctr++;

		// Continue traversing list
	}

	debug(DEBUG_LOW, "create_timeoutq_event. Adding event to end. timeout=", timeout_left);

	// If we haven't found a place for the event, append it to the end. 
	if (!added_event) {
		first_free->timeout = timeout_left;
		LL_APPEND(timeoutq, first_free);
	}
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

	debug(DEBUG_LOW, "Begin handle_timeoutq_event", 0);

	struct event* first = (struct event*) LL_FIRST(timeoutq);

	if (first == NULL) {
		panic(ERRNO_NO_QUEUE_HEAD, "handle_timeoutq_event");
	}

	bring_timeoutq_current();

	if (first->timeout < 2) {
		first->go(first->data);
		LL_POP(timeoutq);

		if (first->repeat_interval) {
			LL_PUSH(freelist, first);
			create_timeoutq_event(first->repeat_interval, first->repeat_interval, first->go, first->data);
		}

		return 1;
	}

	return 0;	
}