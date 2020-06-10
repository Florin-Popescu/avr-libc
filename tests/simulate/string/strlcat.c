/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 * Copyright (c) 2009 Dmitry Xmelkov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Test of strlcat(). The program includes the strlcat() function copy
   for testing themselves.
   $Id: strlcat.c 2033 2009-10-25 07:31:47Z dmix $
 */

#include <stdlib.h>
#include <string.h>
#include "progmem.h"

#ifndef __AVR__
# include <stdio.h>
# define PRINTFLN(line, fmt, ...)	\
    printf("\nLine %d: " fmt "\n", line, ##__VA_ARGS__)
# define EXIT(code)	exit ((code) < 255 ? (code) : 255)
#else
# define PRINTFLN(line, fmt, ...)
# define EXIT	exit
#endif

#ifndef	__AVR__

/* Appends src to string dst of size siz (unlike strncat(), siz is the
   full size of dst, not space left).  At most siz-1 characters will be
   copied.  Always NULL terminates (unless siz <= strlen(dst)).

   The strlcat() function returns strlen(src) + MIN(siz,
   strlen(initial dst)).  If retval >= siz, truncation occurred.
 */

size_t strlcat (char *dst, const char *src, size_t siz)
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));	/* count does not include NUL */
}
#endif	/* !__AVR__ */

void Check (int line,
	    const char *dst, const char *src, size_t size,
	    const char *result)
{
    char d[strlen_P (dst) + strlen_P (src) + 1];
    char s[strlen_P (src) + 1];
    size_t n, must;

    strcpy_P (d, dst);
    strcpy_P (s, src);

    must = (size < strlen_P (dst) ? size : strlen_P (dst)) + strlen_P (src);
    n = strlcat (d, s, size);
    if (n != must) {
	PRINTFLN (line, "wrong return value: %zu, must: %zu", n, must);
	EXIT (line);
    }

    if (strcmp_P (d, result) != 0) {
	PRINTFLN (line, "wrong result dst: %s", d);
	EXIT (1000 + line);
    }
}

#define CHECK(dst, src, size, result)					\
    do {								\
	Check (__LINE__, PSTR (dst), PSTR (src), size, PSTR (result));	\
    } while (0)


int main ()
{
    /* Arg 'size' is 0.	*/
    CHECK ("", "", 0, "");
    CHECK ("1", "", 0, "1");
    CHECK ("23", "", 0, "23");
    CHECK ("", "A", 0, "");
    CHECK ("1", "A", 0, "1");
    CHECK ("23", "A", 0, "23");
    CHECK ("", "BC", 0, "");
    CHECK ("1", "BC", 0, "1");
    CHECK ("23", "BC", 0, "23");

    /* Arg 'size' is 1.	*/
    CHECK ("", "", 1, "");
    CHECK ("1", "", 1, "1");
    CHECK ("23", "", 1, "23");
    CHECK ("", "A", 1, "");
    CHECK ("1", "A", 1, "1");
    CHECK ("23", "A", 1, "23");
    CHECK ("", "BC", 1, "");
    CHECK ("1", "BC", 1, "1");
    CHECK ("23", "BC", 1, "23");

    /* Arg 'size' is 2.	*/
    CHECK ("", "", 2, "");
    CHECK ("1", "", 2, "1");
    CHECK ("23", "", 2, "23");
    CHECK ("456", "", 2, "456");
    CHECK ("", "A", 2, "A");
    CHECK ("1", "A", 2, "1");
    CHECK ("23", "A", 2, "23");
    CHECK ("456", "A", 2, "456");
    CHECK ("", "BC", 2, "B");
    CHECK ("1", "BC", 2, "1");
    CHECK ("23", "BC", 2, "23");
    CHECK ("456", "BC", 2, "456");

    /* Arg 'size' is 3.	*/
    CHECK ("", "", 3, "");
    CHECK ("1", "", 3, "1");
    CHECK ("23", "", 3, "23");
    CHECK ("456", "", 3, "456");
    CHECK ("7890", "", 3, "7890");
    CHECK ("", "A", 3, "A");
    CHECK ("1", "A", 3, "1A");
    CHECK ("23", "A", 3, "23");
    CHECK ("456", "A", 3, "456");
    CHECK ("7890", "A", 3, "7890");
    CHECK ("", "BC", 3, "BC");
    CHECK ("1", "BC", 3, "1B");
    CHECK ("23", "BC", 3, "23");
    CHECK ("456", "BC", 3, "456");
    CHECK ("7890", "BC", 3, "7890");
    CHECK ("", "DEF", 3, "DE");
    CHECK ("1", "DEF", 3, "1D");
    CHECK ("23", "DEF", 3, "23");
    CHECK ("456", "DEF", 3, "456");
    CHECK ("7890", "DEF", 3, "7890");

    /* Margin chars.	*/
    CHECK ("\001", "\377", 10, "\001\377");
    CHECK ("\377", "\001", 20, "\377\001");

    /* Arg 'size' is very big.	*/
    CHECK ("123", "ABC", 32767, "123ABC");
    CHECK ("456", "D", 32768, "456D");
    CHECK ("0", "EFG", 65535, "0EFG");

    CHECK ("The quick ", "brown fox", 19, "The quick brown fo");

#if	!defined (RAMEND) || RAMEND > 0x400

    /* Arg 'size' is 256.	*/
    CHECK ("................................................................"
	   "................................................................"
	   "................................................................"
	   ".............................................................4",
	   "A", 256,
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   ".............................................................4A");
    CHECK ("................................................................"
	   "................................................................"
	   "................................................................"
	   "..............................................................5",
	   "B", 256,
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "..............................................................5");
    CHECK ("C",
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   ".............................................................4",
	   256,
	   "C..............................................................."
	   "................................................................"
	   "................................................................"
	   "..............................................................4");
    CHECK ("D",
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "..............................................................5",
	   256,
	   "D..............................................................."
	   "................................................................"
	   "................................................................"
	   "...............................................................");

    /* Arg 'size' is 257.	*/
    CHECK ("................................................................"
	   "................................................................"
	   "................................................................"
	   "..............................................................5",
	   "E", 257,
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "..............................................................5E");
    CHECK ("................................................................"
	   "................................................................"
	   "................................................................"
	   "...............................................................6",
	   "F", 257,
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "...............................................................6");
    CHECK ("G",
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "..............................................................5",
	   257,
	   "G..............................................................."
	   "................................................................"
	   "................................................................"
	   "...............................................................5");
    CHECK ("H",
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "...............................................................6",
	   257,
	   "H..............................................................."
	   "................................................................"
	   "................................................................"
	   "................................................................");

    /* Arg 'size' is 258.	*/
    CHECK ("................................................................"
	   "................................................................"
	   "................................................................"
	   "...............................................................6",
	   "I", 258,
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "...............................................................6I");
    CHECK ("................................................................"
	   "................................................................"
	   "................................................................"
	   "................................................................7",
	   "J", 258,
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "................................................................7");
    CHECK ("K",
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "...............................................................6",
	   258,
	   "K..............................................................."
	   "................................................................"
	   "................................................................"
	   "................................................................6");
    CHECK ("L",
	   "................................................................"
	   "................................................................"
	   "................................................................"
	   "................................................................7",
	   258,
	   "L..............................................................."
	   "................................................................"
	   "................................................................"
	   ".................................................................");
#endif

    return 0;
}
