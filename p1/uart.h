extern unsigned int uart_lcr ( void );
extern unsigned int uart_recv ( void );
extern unsigned int uart_check ( void );
extern void uart_send ( unsigned int );
extern void uart_flush ( void );
extern void uart_put32x ( unsigned int );
extern void uart_put64x ( unsigned long long );
extern void uart_puts ( char * );
extern void init_uart ( void );
