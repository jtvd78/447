
//
// terminal I/O example
//



#include "utils.h"
#include "time.h"
#include "led.h"
#include "uart.h"


int
notmain ( void )
{
	int ch;

	// init funcs
    init_uart();
	init_timer();

	// finished initialization
	blink_led(10);

	// do some work
    uart_puts("\n");
    uart_puts("\n");
    uart_puts("Justin's system is booting.\n");
    uart_puts("Please hit enter to continue.\n");
    uart_puts("\n");

    ch = uart_recv();

    uart_puts("Running datasize test. Lengths in nibbles.\n");
    {
    	char c;
		short s;
		int i,j;
		long l;
		long long x;

		uart_puts("char      ");
		for (j=0, c=-1; j<18 && c!=0; j++) {
			c = (c << 4) & 0xfffffffffffffff0;
		}
		uart_put32x(j); uart_puts("\n");

		uart_puts("short     ");
		for (j=0, s=-1; j<18 && s!=0; j++) {
			s = (s << 4) & 0xfffffffffffffff0;
		}
		uart_put32x(j); uart_puts("\n");

		uart_puts("int       ");
		for (j=0, i=-1; j<18 && i!=0; j++) {
			i = (i << 4) & 0xfffffffffffffff0;
		}
		uart_put32x(j); uart_puts("\n");

		uart_puts("long      ");
		for (j=0, l=-1; j<18 && l!=0; j++) {
			l = (l << 4) & 0xfffffffffffffff0;
		}
		uart_put32x(j); uart_puts("\n");

		uart_puts("long long ");
		for (j=0, x=-1; j<18 && x!=0; j++) {
			x = (x << 4) & 0xfffffffffffffff0;
		}
		uart_put32x(j); uart_puts("\n");

		uart_puts("\n");
    }

    uart_puts("Running pointer test. Length in bytes.\n");
	uart_puts("void *    ");

	void * ptr = GETPC();
	uart_put64x(sizeof(ptr)); uart_puts("\n");
	uart_puts("PC value: ");
	uart_put64x((unsigned long long)ptr); uart_puts("\n");
	uart_puts("\n");


    uart_puts("Running ECHO server.\n");
    while (1) {
		while(1) {
			if((now_usec()&0x00007000)==0x00007000) {
			break;
		}}
		ch=uart_recv();

		/* Convert to uppercase */
		if ((ch>='a') && (ch<='z')) ch-=32;

		while(1) {
			if((now_usec()&0x00007000)==0x00007000) {
			break;
		}}
		PUT32(0x3F215040,ch);

		if (ch < 20) {
			while(1) {
				if((now_usec()&0x00007000)==0x00007000) {
					break;
			}}
			PUT32(0x3F215040,0x0A);
		}
    }

    return(0);
}


//-------------------------------------------------------------------------
//
// Copyright (c) 2016 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
