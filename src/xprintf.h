/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2011     */
/*------------------------------------------------------------------------*/
#ifndef _STRFUNC
#define _STRFUNC

#include <stdarg.h>

#define _USE_XFUNC_OUT	1	/* 1: Use output functions */
#define	_CR_CRLF		1	/* 1: Convert \n ==> \r\n in the output char */

#define _USE_XFUNC_IN	0	/* 1: Use input function */
#define	_LINE_ECHO		0	/* 1: Echo back input chars in xgets function */


#if _USE_XFUNC_OUT
void xputc   (char c);
void xputs   (const char* str);

#ifdef __cplusplus
extern "C"
#endif
void xprintf (const char* fmt, ...);

void xsprintf(char* buff, const char* fmt, ...);
void xvsprintf (char* buff, const char*	fmt, va_list arp);

#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)
#endif

#if _USE_XFUNC_IN
int xgetc( void );
int xgets(char* buff, int len);
int xatoi(char** str, long* res);
#endif

#endif
