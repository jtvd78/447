
#define LSH(x,y)			(((unsigned long)(x & 0xff)) << y)
#define	CMD_NUM(a,b,c,d)	(unsigned long)(LSH(a,0) | LSH(b,8) | LSH(c,16) | LSH(d,24))

#define	CMD_LOG		CMD_NUM('L','O','G','\0')
#define	CMD_TIME	CMD_NUM('T','I','M','E')
#define	CMD_LED		CMD_NUM('L','E','D','\0')
#define	CMD_EXIT	CMD_NUM('E','X','I','T')

