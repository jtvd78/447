// Little Endian

#include "utils.h"

#define TIMER_CONTROL  0x40000000
#define TIMER_PRESCALE 0x40000008

#define TIMER_LOW  0x4000001C
#define TIMER_HIGH 0x40000020 

unsigned long long now_usec ()
{
	unsigned long long low  = GET32(TIMER_LOW);
	unsigned long long high = GET32(TIMER_HIGH);
	return (high << 32) + low;
}

unsigned long long usec_diff ( unsigned long long now, unsigned long long before )
{
	if (now < before)
	{
		return (0xFFFFFFFFFFFFFFFFull - before) + now + 1;
	}

	return now - before;
}

void wait( unsigned int usecs )
{
	
	long long time_left = usecs;
	unsigned long long before = now_usec();
	unsigned long long now = before;

	while((time_left = time_left - usec_diff(now, before)) > 0)
	{
		before = now;
		now = now_usec();
	}

	return;
}

void init_timer()
{
	// Bit 8 -> 0 : 64-bit Core timer runs from the Crystal clock
	// Bit 9 -> 0 : 64-bit Core timer increments by 1
	PUT32(TIMER_CONTROL, 0);

	// Sets timer to 1MHz (Divider is 19.2)
	PUT32(TIMER_PRESCALE, 0x06AAAAAB);

	// Reset timer to 0
	PUT32(TIMER_LOW, 0);
	PUT32(TIMER_HIGH, 0);

	return;
}