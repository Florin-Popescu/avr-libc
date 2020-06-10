/* Copyright (c) 2007  Dmitry Xmelkov
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
 */

/* Test of division.  Subnormals.
   $Id: div-02.c 1923 2009-03-07 14:02:24Z dmix $
 */
#include <stdio.h>
#include <stdlib.h>
#include "progmem.h"

union lofl_u {
    long lo;
    float fl;
};

volatile union lofl_u v = { .lo = 1 };

PROGMEM const struct {		/* Table of test cases:  x / y = z	*/
    union lofl_u x, y, z;
} t[] = {

    { { 0x00000001 }, { 0x00000001 }, { .fl = 1 } },
    { { 0x00000002 }, { 0x00000001 }, { .fl = 2 } },
    { { 0x00000003 }, { 0x00000001 }, { .fl = 3 } },
    { { 0x00000004 }, { 0x00000001 }, { .fl = 4 } },
    { { 0x007fffff }, { 0x00000001 }, { .fl = 0x0.7fffffp+24 } },
    { { 0x00800000 }, { 0x00000001 }, { .fl = 0x0.800000p+24 } },
    { { 0x00800001 }, { 0x00000001 }, { .fl = 0x0.800001p+24 } },
    { { 0x00800002 }, { 0x00000001 }, { .fl = 0x0.800002p+24 } },
    { { 0x00ffffff }, { 0x00000001 }, { .fl = 0x0.ffffffp+24 } },
    { { 0x01000000 }, { 0x00000001 }, { .fl = 0x0.800000p+25 } },
    { { 0x34fffffe }, { 0x00000001 }, { 0x7f7ffffe } },
    { { 0x34ffffff }, { 0x00000001 }, { 0x7f7fffff } },
    { { 0x35000000 }, { 0x00000001 }, { 0x7f800000 } },
    { { 0x35000001 }, { 0x00000001 }, { 0x7f800000 } },
    { { 0x7f7fffff }, { 0x00000001 }, { 0x7f800000 } },

    { { 0x00000001 }, { 0x00000001 }, { .fl = 1 } },
    { { 0x00000001 }, { 0x00000002 }, { .fl = 0.5 } },
    { { 0x00000001 }, { 0x00000003 }, { .fl = 0.33333333 } },
    { { 0x00000001 }, { 0x00000004 }, { .fl = 0.25 } },
    { { 0x00000001 }, { 0x00100000 }, { .fl = 0x1p-20 } },
    { { 0x00000001 }, { 0x00800000 }, { .fl = 0x1p-23 } },
    { { 0x00000001 }, { .fl = 1.0  }, { .fl = 0x1p-149 } },
    { { 0x00000001 }, { .fl = 2.0  }, { .fl = 0 } },
};

void x_exit (int index)
{
#ifndef	__AVR__
    fprintf (stderr, "t[%d]:  %#lx\n", index - 1, v.lo);
#endif
    exit (index ? index : -1);
}

int main ()
{
    union lofl_u x,y,z;
    int i;
    
    for (i = 0; i < (int) (sizeof(t) / sizeof(t[0])); i++) {
	x.lo = pgm_read_dword (& t[i].x);
	y.lo = pgm_read_dword (& t[i].y);
	z.lo = pgm_read_dword (& t[i].z);
	v.fl = x.fl / y.fl;
	/* Comparison is integer to verify the zero sign.	*/
	if (v.lo != z.lo)
	    x_exit (i+1);
    }
    return 0;
}
