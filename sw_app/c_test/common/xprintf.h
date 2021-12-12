

#ifndef _XPRINTF_H_
#define _XPRINTF_H_

#define	_CR_CRLF		0	/* 1: Convert \n ==> \r\n in the output char */

int put_char (char c);
int put_str (const char* str);
void put_hex(unsigned int h);
void xprintf (const char* fmt, ...);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)

#endif
