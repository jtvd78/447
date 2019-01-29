// Little Endian

#include "utils.h"

#define TIMER_CONTROL  0x40000000
#define TIMER_PRESCALE 0x40000008

#define TIMER_LOW  0x4000001C
#define TIMER_HIGH 0x40000020 

unsigned long long int now_usec ()
{
	return (((unsigned long long int) GET32(TIMER_HIGH)) << 32) + ((unsigned long long int) GET32(TIMER_LOW));
}

unsigned long long int usec_diff ( unsigned long long int now, unsigned long long int before )
{
	if (now < before)
	{
		return (0xFFFFFFFFFFFFFFFFLL - before) + now;
	}

	return now - before;
}

void wait( unsigned int usecs )
{
	
	long long int time_left = usecs;
	
	unsigned long long int before = now_usec();
	while(1)
	{
		unsigned long long int now = now_usec();
		time_left -= usec_diff(now, before);

		if (time_left <= 0) {
			break;
		}

		before = now;
	}

	return;
}

void init_timer()
{
	// Bit 8 -> 0 : 64-bit Core timer runs from the Crystal clock
	// Bit 9 -> 0 : 64-bit Core timer increments by 1
	PUT32(TIMER_CONTROL, 0);

	// Sets timer to 1MHz
	PUT32(TIMER_PRESCALE, 0x06AAAAAB);

	// Reset timer to 0
	PUT32(TIMER_LOW, 0);
	PUT32(TIMER_HIGH, 0);

	return;
}