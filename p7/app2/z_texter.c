
#include "../syscall.h"
#include "../dev.h"
#include "u_text.h"



char *tens[] = {
	"",
	"ten and ",
	"twenty ",
	"thirty ",
	"forty ",
	"fifty ",
	"sixty ",
	"seventy ",
	"eighty ",
	"ninety ",
};

char *ones[] = {
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
};

char name[] = "Texter: ";

void
notmain( )
{
	uint64_t now_usec;
	uint64_t wait_until;
	int num = 0;
	char buffer[64];
	int t, o;
	char *cp;

	if (0) puts("\nRunning texter.\n");

	while (1) {
		o = num % 10;
		t = num / 10;
		num = (num + 1) % 100;

		cp = buffer;
		strcpyN(cp, name, 64);
		cp += strlen(name);
		strcpyN(cp, tens[t], 10);
		cp += strlen(tens[t]);
		strcpyN(cp, ones[o], 10);

		syscall_write_cbuffer(DEV_KLOG, buffer, 64);

		syscall_read_cbuffer(DEV_CLOCK, &now_usec, 8);
		wait_until = now_usec + (2000 * 1000);

		while (now_usec < wait_until) {
			syscall_read_cbuffer(DEV_CLOCK, &now_usec, 8);
		}

	}
}
