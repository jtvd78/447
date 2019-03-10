
#include "os.h"

#define CLOCK_BASE	0x3F003000

#define	CLOCK_CTL	CLOCK_BASE+0x00
#define	CLOCK_LO	CLOCK_BASE+0x04
#define	CLOCK_HI	CLOCK_BASE+0x08
#define	CLOCK_CMP0	CLOCK_BASE+0x0c
#define	CLOCK_CMP1	CLOCK_BASE+0x10
#define	CLOCK_CMP2	CLOCK_BASE+0x14
#define	CLOCK_CMP3	CLOCK_BASE+0x18

#define INT_BASE	0x3F00B000

#define	TIMER_Load			(INT_BASE + 0x400)
#define	TIMER_Value 		(INT_BASE + 0x404)
#define	TIMER_Control		(INT_BASE + 0x408)
#define	TIMER_IRQ_ClrAck	(INT_BASE + 0x40C)
#define	TIMER_RAW_IRQ		(INT_BASE + 0x410)
#define	TIMER_Masked_IRQ	(INT_BASE + 0x414)
#define	TIMER_Reload		(INT_BASE + 0x418)
#define	TIMER_Predivider	(INT_BASE + 0x41C)

#define	IRQ_Basic_Pending	(INT_BASE + 0x200)
#define	IRQ_Pending_1		(INT_BASE + 0x204)
#define	IRQ_Pending_2		(INT_BASE + 0x208)
#define	FIQ_Control			(INT_BASE + 0x20C)
#define	Enable_IRQs_1		(INT_BASE + 0x210)
#define	Enable_IRQs_2		(INT_BASE + 0x214)
#define	Enable_Basic_IRQs	(INT_BASE + 0x218)
#define	Disable_IRQs_1		(INT_BASE + 0x21C)
#define	Disable_IRQs_2		(INT_BASE + 0x220)
#define	Disable_Basic_IRQs	(INT_BASE + 0x224)

uint64_t
get_time( )
{
	unsigned long hi, lo;
	uint64_t thi, tlo;

	hi = GET32(CLOCK_HI);
	lo = GET32(CLOCK_LO);
	if (hi != GET32(CLOCK_HI)) {
		hi = GET32(CLOCK_HI);
		lo = GET32(CLOCK_LO);
	}

	thi = (((uint64_t)hi) << 32) & 0xFFFFFFFF00000000;
	tlo = ((uint64_t)lo) & 0x00000000FFFFFFFF;

	return thi | tlo;
}

unsigned long
now_usec () 
{
	return GET32(CLOCK_LO);
}

unsigned long
now_hrs () 
{
	return GET32(CLOCK_HI);
}

void
clear_timer_interrupts()
{
	return;
}

void
wait( unsigned long usecs )
{
	uint64_t until = (uint64_t)usecs + get_time();

	while (get_time() < until)
	;

	return;
}

void init_time()
{
//	unsigned old = GET32(TIMER_Control);
//	old &= ~(0x20 | 0x80);			// disable timer, disable interrupt
//	PUT32(TIMER_Control, old);
	PUT32(TIMER_Predivider, 0xF9);

	PUT32(TIMER_Control,   ((999 << 16) |	// 1000x downsample
							(0x1 << 5) |	// timer interrupt enabled
							(0x1 << 7) |	// timer enabled
							(0x1 << 9) |	// free-running counter enabled
							(0x1 << 1) ));	// 23-bit counter

	return;
}

void
clear_timer_interrupt()
{
	PUT32(TIMER_IRQ_ClrAck, 0x1);
}

void
set_timer()
{
	PUT32(TIMER_Load, 10);	// time in millisecs

	PUT32(Enable_IRQs_1, 64);
	PUT32(Enable_Basic_IRQs, 0x1);

	enable_irq();
	return;
}
