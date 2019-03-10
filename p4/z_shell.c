
#include "syscall.h"
#include "u_text.h"
#include "dev.h"

typedef union namenum {
	char	name[4];
	short	half[2];
	long	num;
	void	*ptr;
	float	fnum;
} namenum_t;

#define LSH(x,y)	(((unsigned long)(x & 0xff)) << y)
#define	SYSCALL_NUM(a,b,c,d)	(unsigned long)(LSH(a,0) | LSH(b,8) | LSH(c,16) | LSH(d,24))

#define	CMD_LOG		SYSCALL_NUM('L','O','G','\0')
#define	CMD_TIME	SYSCALL_NUM('T','I','M','E')
#define	CMD_LED		SYSCALL_NUM('L','E','D','\0')
#define	CMD_EXIT	SYSCALL_NUM('E','X','I','T')


long shell_regs[30];

char input_buffer[1024];

void
puts ( char *c )
{
    unsigned int rc;

    while (*c) {
    	rc = *c++;
		if ((rc & 0xff) == '\n') {
			syscall_write_word(DEV_CONSOLE, (unsigned long)'\r');
		}
		syscall_write_word(DEV_CONSOLE, rc);
    }
}

void
put32x ( unsigned int d )
{
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        syscall_write_word(DEV_CONSOLE, rc);
        if(rb==0) break;
    }
}

void
run_shell()
{
	int i,ch,led=0;
	char *cp;
	char *next;
	namenum_t cmd[5];
	char name[5];
	uint64_t time_usec;

	puts("\nRunning shell.\n");

//  useful for testing syscalls individually
//	syscall_get_kernel_version(input_buffer);
//	ch = syscall_read_word(DEV_CONSOLE);
//	while ((ch = syscall_read_word(DEV_CONSOLE)) >= 0) {
//		syscall_write_word(DEV_CONSOLE, ch);
//	}
//	while (1) {
//		syscall_write_word(DEV_CONSOLE, (unsigned long)'.');
//		for (i=0; i<500000; i++) dummy();
//	}

	puts("Available commands:\n");
		puts(" LED  = "); put32x(CMD_LED); puts("\n");
		puts(" LOG  = "); put32x(CMD_LOG); puts("\n");
		puts(" TIME = "); put32x(CMD_TIME); puts("\n");
		puts(" EXIT = "); put32x(CMD_EXIT); puts("\n");

extern void dummy();
	while (1) {
		puts("\nPlease enter a command.\n> ");


		// comment this out if you want to see what happens
		// when you go into the kernel and try to READ the UART.
		for (i=0; i<100000000; i++) {
			asm volatile ("nop");
		}
		continue;


		//
		// reads in characters until newline
		//
		for (i=0; (ch=syscall_read_word(DEV_CONSOLE)) >= 0x20; i++) {
			/* Convert to uppercase */
			if ((ch>='a') && (ch<='z')) ch-=32;

			syscall_write_word(DEV_CONSOLE, ch);

			input_buffer[i] = (char)(ch & 0xff);
			input_buffer[i+1] = '\0';
		}
		puts("\n");

		//
		// parses the line of characters
		//
		for (i=0, cp = input_buffer, next = tokenize(input_buffer, ' ');
			i < 4 && cp != NULL;
			next = tokenize((cp[0] == '"' ? cp+1 : cp), (cp[0] == '"' ? '"' : ' ')), i++) {

			if (cp[0] == '0' && cp[1] == 'X') {
				// assume it's a hexadecimal value
				cmd[i].num = atol_hex(cp);
			} else if (cp[0] == '"') {
				cmd[i].ptr = cp+1;
			} else if (cp[0] >= '0' && cp[0] <= '9') {
				// assume it's a decimal value
				cmd[i].num = atol_dec(cp);
			} else {
				cmd[i].num = 0x00000000;
				strcpy4(cmd[i].name, cp);
			}
			cmd[i+1].num = 0;
			cp = next;
		}

		//
		// handles the command
		//
		switch(cmd[0].num) {
			case CMD_LOG:
				puts("CMD_LOG [");
				if (cmd[1].ptr) puts(cmd[1].ptr);
				puts("]\n");
				syscall_write_stream(DEV_KLOG, cmd[1].ptr, CBUFSIZE);
				break;
			case CMD_TIME:
				puts("CMD_TIME = [");
				syscall_read_stream(DEV_CLOCK, &time_usec, 8);
				put32x((long)((time_usec >> 32) & 0xffffffff));
				puts(" ");
				put32x((long)(time_usec & 0xffffffff));
				puts("]\n");
				break;
			case CMD_LED:
				if (led) {
					puts("CMD_LED - off\n");
					syscall_write_word(DEV_LED, 0);
				} else {
					puts("CMD_LED - on\n");
					syscall_write_word(DEV_LED, 1);
				}
				led = ~led;
				break;
			case CMD_EXIT:
				puts("CMD_EXIT exiting ...\n");
				return;
				break;
			default:
				strcpy4(name, cmd[0].name);
				name[4] = '\0';
				puts("unknown command: ");
				puts(name);
				puts(" - ");
				put32x(cmd[0].num);
				puts("\n");
				break;
		}
    }

	return;
}
