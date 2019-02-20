/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "time.h"
#include "utils.h"

//
// set this up if we ever want to use countdown timers
//
void
init_timer()
{
}


/**
 * Wait N CPU cycles (ARM CPU only)
 */
void wait_cycles(unsigned int n)
{
    if(n) while(n--) { asm volatile("nop"); }
}

/**
 * Wait N microsec (ARM CPU only)
 */
time_t get_time()
{
    register unsigned long t;
    // read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    return (time_t)t;
}

//
// inelegant but effective
//
// assumes the system timer is running at 1 tick per microsecond
//
// in the future, we will want to enforce a max wait time of 1/100 sec
// where we periodically come out of it and check to see if any interrupts
// or messages have come in
//
void
wait ( time_t usec )
{
	usec += get_time();

	while (get_time() < usec)
	;

	return;
}

/**
 * Wait N microsec (ARM CPU only)
 */
void var_wait(time_t n)
{
    register unsigned long f, t, r;
    // get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // calculate expire value for counter
    t+=((f/1000)*n)/1000;
    do{asm volatile ("mrs %0, cntpct_el0" : "=r"(r));}while(r<t);
}
