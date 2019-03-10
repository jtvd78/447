
typedef unsigned long long uint64_t;

enum syscalls {
	SYSCALL_INVALID,
	SYSCALL_KVERSION,
	SYSCALL_RD_WORD,
	SYSCALL_WR_WORD,
	SYSCALL_RD_STREAM,
	SYSCALL_WR_STREAM,
	// insert new ones here
	MAX_SYSCALLS,
};

int syscall_get_kernel_version(void *buffer);
int syscall_read_word(int device);
int syscall_write_word(int device, long data);
int syscall_read_stream(int device, void *buffer, int bufsize);
int syscall_write_stream(int device, void *buffer, int bytes);

