
#define DEAD	0xdeadbeef
#define NULL	0

int is_printing(char c);
long atol_dec(char *s);
long atol_hex(char *s);
char * tokenize(char *s, char);
void strcpy4(char *to, char *from);
int strcpyN(char *to, char *from, int bufsiz);
int strlen(char *s);

void puts ( char *c );
void put32x ( unsigned int d );
void put4x ( unsigned int d );
