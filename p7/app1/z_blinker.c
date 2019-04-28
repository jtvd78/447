
#include "../syscall.h"
#include "../dev.h"

void
flash_led(int reps)
{
	uint64_t now_usec;
	uint64_t wait_until;

	while (reps-- > 0) {
		syscall_write_word(DEV_LED, 1);

		syscall_read_cbuffer(DEV_CLOCK, &now_usec, 8);
		wait_until = now_usec + (50 * 1000);

		while (now_usec < wait_until) {
			syscall_read_cbuffer(DEV_CLOCK, &now_usec, 8);
		}

		syscall_write_word(DEV_LED, 0);

		syscall_read_cbuffer(DEV_CLOCK, &now_usec, 8);
		wait_until = now_usec + (50 * 1000);

		while (now_usec < wait_until) {
			syscall_read_cbuffer(DEV_CLOCK, &now_usec, 8);
		}
	}
}

extern void puts(char *s);

void
notmain( )
{
	uint64_t now_usec;
	uint64_t wait_until;
	int num = 0;

	if (0) puts("\nRunning blinker.\n");

	while (1) {
		flash_led(++num);
		num = num & 0x3;

		syscall_read_cbuffer(DEV_CLOCK, &now_usec, 8);
		wait_until = now_usec + (700 * 1000);

		while (now_usec < wait_until) {
			syscall_read_cbuffer(DEV_CLOCK, &now_usec, 8);
		}

	}
}
