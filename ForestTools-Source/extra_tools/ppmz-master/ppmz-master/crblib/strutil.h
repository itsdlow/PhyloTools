
#ifndef CRB_STRUTIL_H
#define CRB_STRUTIL_H

#include <string.h>

#ifdef __WATCOMC__
#define IN_COMPILER
#endif

#ifdef __BORLANDC__
#define IN_COMPILER
#endif

#ifdef _AMIGA
#define IN_COMPILER
#endif

#include <crblib/inc.h>

extern char * getParam(char *arg,char *argv[],int *iPtr);

extern char * nexttok(char *str); // modifies str!
extern char * find_numeric(char *str);
extern char * skipwhitespace(char * str);

extern void strcommas(char *str,ulong num);

/* str*() like functions, but for the ubyte memory region */
extern int blocklen(ubyte *ptr);
ubyte* blockcpy(void *vdst, void *vsrc);
ubyte* blockncpy(void *vdst, void *vsrc, int len);
int blockcmp(void *s1, void *s2);
int blockncmp(void *s1, void *s2, int len);
ubyte* blockchr(ubyte *s, unsigned int c);

extern char * strichr(const char *str,short c);
extern char * stristr(const char *bigstr,const char *substr);
extern char * strnchr(const char *str,short c,int len);
extern void strtolower(char *str);
extern void strins(char *to,const char *fm);
	 /* strins drops fm at head of to */
extern void strrep(unsigned char *str,char fm,char to);

#ifndef IN_COMPILER
extern int stricmp(const char *str,const char *vs);
extern char *strupr(char *str);
extern char *strrev(char *str);
#endif

#define strlwr strtolower
#define strlower strtolower
#define strupper strupr

#endif

