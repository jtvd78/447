
typedef unsigned long long uint64_t;

enum syscalls {
	SYSCALL_INVALID,
	SYSCALL_KVERSION,
	SYSCALL_RD_WORD,
	SYSCALL_WR_WORD,
	SYSCALL_RD_CBUF,
	SYSCALL_WR_CBUF,
	SYSCALL_RD_BLOCK,
	SYSCALL_WR_BLOCK,
	SYSCALL_START_THREAD,
	SYSCALL_PROCESSES,
	SYSCALL_DUMP_RANGE,
	// insert new ones here
	MAX_SYSCALLS,
};

int syscall_get_kernel_version(void *buffer);
int syscall_read_word(int device);
int syscall_write_word(int device, long data);
int syscall_read_cbuffer(int device, void *buffer, int bufsize);
int syscall_write_cbuffer(int device, void *buffer, int bytes);
int syscall_read_block(int device, void *buffer, int bufsize);
int syscall_write_block(int device, void *buffer, int bytes);
int syscall_start_thread(long name, long start_address);
int syscall_list_processes();
int syscall_dump_range(long start_addr, long end_address, long width);

