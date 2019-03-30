
#include "syscall.h"

int
syscall_get_kernel_version(void *buffer)
{
	register long r7 asm("r7") = SYSCALL_KVERSION;
	register long r0 asm("r0") = (long)buffer;

	asm volatile (
		"svc #1\n"
		: "=r"(r0)			// output: return status
		: "r"(r0), "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
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
syscall_read_cbuffer(int device, void *buffer, int bufsize)
{
	register long r7 asm("r7") = SYSCALL_RD_CBUF;
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
syscall_write_cbuffer(int device, void *buffer, int bytes)
{
	register long r7 asm("r7") = SYSCALL_WR_CBUF;
	register long r0 asm("r0") = device;
	register long r1 asm("r1") = (long)buffer;
	register long r2 asm("r2") = bytes;

	asm volatile (
		"svc #5\n"
		: "=r"(r0)								// output: return status
		: "r"(r0), "r"(r1), "r"(r2), "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
}

int
syscall_read_block(int device, void *buffer, int bufsize)
{
	register long r7 asm("r7") = SYSCALL_RD_BLOCK;
	register long r0 asm("r0") = device;
	register long r1 asm("r1") = (long)buffer;
	register long r2 asm("r2") = bufsize;

	asm volatile (
		"svc #6\n"
		: "=r"(r0)								// output: return status
		: "r"(r0), "r"(r1), "r"(r2), "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
}

int
syscall_write_block(int device, void *buffer, int bytes)
{
	register long r7 asm("r7") = SYSCALL_WR_BLOCK;
	register long r0 asm("r0") = device;
	register long r1 asm("r1") = (long)buffer;
	register long r2 asm("r2") = bytes;

	asm volatile (
		"svc #7\n"
		: "=r"(r0)								// output: return status
		: "r"(r0), "r"(r1), "r"(r2), "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
}

int
syscall_start_thread(long name, long start_addr)
{
	register long r7 asm("r7") = SYSCALL_START_THREAD;
	register long r0 asm("r0") = name;
	register long r1 asm("r1") = start_addr;

	asm volatile (
		"svc #7\n"
		: "=r"(r0)					// output: return status
		: "r"(r0), "r"(r1), "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
}

int
syscall_list_processes()
{
	register long r7 asm("r7") = SYSCALL_PROCESSES;
	register long r0 asm("r0");

	asm volatile (
		"svc #7\n"
		: "=r"(r0)	// output: return status
		: "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
}

int
syscall_dump_range(long start_addr, long end_addr, long width)
{
	register long r7 asm("r7") = SYSCALL_DUMP_RANGE;
	register long r0 asm("r0") = start_addr;
	register long r1 asm("r1") = end_addr;
	register long r2 asm("r2") = width;

	asm volatile (
		"svc #7\n"
		: "=r"(r0)								// output: return status
		: "r"(r0), "r"(r1), "r"(r2), "r"(r7)	// input: syscall & arg/s
		: "memory");

	return r0;
}
