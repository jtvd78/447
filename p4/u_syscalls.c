
#include "syscall.h"

int
syscall_get_kernel_version(void *buffer)
{
	// your code goes here
}

int
syscall_read_word(int device)
{
	register long r7 asm("r7") = SYSCALL_RD_WORD;
	register long r0 asm("r0") = device;

	asm volatile (
		"svc #2\n"
		: "=r"(r0)			// output: return readval
		: "r"(r0), "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
}

int
syscall_write_word(int device, long data)
{
	register long r7 asm("r7") = SYSCALL_WR_WORD;
	register long r0 asm("r0") = device;
	register long r1 asm("r1") = data;

	asm volatile (
		"svc #3\n"
		: "=r"(r0)					// output: return status
		: "r"(r0), "r"(r1), "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
}

int
syscall_read_stream(int device, void *buffer, int bufsize)
{
	register long r7 asm("r7") = SYSCALL_RD_STREAM;
	register long r0 asm("r0") = device;
	register long r1 asm("r1") = (long)buffer;
	register long r2 asm("r2") = bufsize;

	asm volatile (
		"svc #4\n"
		: "=r"(r0)								// output: return status
		: "r"(r0), "r"(r1), "r"(r2), "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
}

int
syscall_write_stream(int device, void *buffer, int bytes)
{
	// your code goes here
}
