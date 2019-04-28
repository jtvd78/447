
#include "../syscall.h"
#include "../dev.h"
#include "u_text.h"

long
atol_dec(char *s)
{
	long num = DEAD;
	if (s == NULL || *s == '\0') return DEAD;
	num = 0;
	while (*s != '\0') {
		if (*s < '0' || *s > '9') return DEAD;
		num = num * 10 + (long)(*s - '0');
		s++;
	}
	return num;
}

long
atol_hex(char *s)
{
	long num = DEAD;
	if (s == NULL || *s == '\0') return DEAD;
	num = 0;
	if (s[1] == 'x' || s[1] == 'X') s += 2;
	while (*s != '\0') {
		long val=0;
		if (*s >= '0' && *s <= '9') {
			val = (long)(*s - 48);
		} else if (*s >= 'a' && *s <= 'f') {
			val = 10 + (long)(*s - 'a');
		} else if (*s >= 'A' && *s <= 'F') {
			val = 10 + (long)(*s - 'A');
		} else {
			return DEAD;
		}
		num = num * 16 + val;
		s++;
	}
	return num;
}

char *
tokenize(char *s, char tok)
{
	int got_token = 0;
	if (s == NULL) return s;
	while (*s != '\0') {
		while (*s == tok) {
			*s++ = '\0';
			got_token = 1;
		}
		if (got_token) break;
		s++;
	}
	return (*s == '\0' ? NULL : s);
}

void
strcpy4(char *to, char *from)
{
	int i;
	to[1] = to[2] = to[3] = '\0';
	for (i=0; from[i] != '\0' && i<4; i++) {
		to[i] = from[i];
	}
}

int
strlen(char *s)
{
	int i;
	for (i=0; s[i] != '\0' && i<64; i++) 
		;
	return i;
}

int
strcpyN(char *to, char *from, int bufsiz)
{
	int i;
	for (i=0; from[i] != '\0' && i<(bufsiz-1); i++) {
		to[i] = from[i];
	}
	to[i] = '\0';
	to[bufsiz-1] = '\0';
	return i;
}

int
is_printing(char c)
{
	return (c >= 0x20 && c <= 0x7e);
}

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
put4x ( unsigned int d )
{
    unsigned int rc;

	rc = d & 0xF;
	if(rc>9) rc+=0x37; else rc+=0x30;
	syscall_write_word(DEV_CONSOLE, rc);
}
