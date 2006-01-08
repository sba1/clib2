/*
 * $Id$
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2006 by Olaf Barthel <olsen (at) sourcery.han.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Neither the name of Olaf Barthel nor the names of contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * PowerPC math library based in part on work by Sun Microsystems
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 */

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

/****************************************************************************/

#if defined(FLOATING_POINT_SUPPORT)

/****************************************************************************/

#if defined(IEEE_FLOATING_POINT_SUPPORT)

/****************************************************************************/

#if defined(__GNUC__)

/****************************************************************************/

#if defined(SMALL_DATA)
#define A4(x) "a4@(" #x ":W)"
#elif defined(SMALL_DATA32)
#define A4(x) "a4@(" #x ":L)"
#else
#define A4(x) #x
#endif /* SMALL_DATA */

/****************************************************************************/

extern double __acos(double x);

/****************************************************************************/

asm("

	.text
	.even

	.globl	_MathIeeeDoubTransBase
	.globl	___acos

___acos:

	movel	a6,sp@-
	movel	"A4(_MathIeeeDoubTransBase)",a6
	moveml	sp@(8),d0/d1
	jsr		a6@(-120:W)
	movel	sp@+,a6
	rts

");

/****************************************************************************/

#else

/****************************************************************************/

INLINE STATIC const double
__acos(double x)
{
	double result;

	result = IEEEDPAcos(x);

	return(result);
}

/****************************************************************************/

#endif /* __GNUC__ */

/****************************************************************************/

#endif /* IEEE_FLOATING_POINT_SUPPORT */

/****************************************************************************/

#if defined(M68881_FLOATING_POINT_SUPPORT)

INLINE STATIC const double
__acos(double x)
{
	double result;

	__asm ("facos%.x %1,%0"
	       : "=f" (result)
	       : "f" (x));

	return(result);
}

#endif /* M68881_FLOATING_POINT_SUPPORT */

/****************************************************************************/

#if defined(PPC_FLOATING_POINT_SUPPORT)

static const double 
one=  1.00000000000000000000e+00,      /* 0x3FF00000, 0x00000000 */
pi =  3.14159265358979311600e+00,      /* 0x400921FB, 0x54442D18 */
pio2_hi =  1.57079632679489655800e+00, /* 0x3FF921FB, 0x54442D18 */
pio2_lo =  6.12323399573676603587e-17, /* 0x3C91A626, 0x33145C07 */
pS0 =  1.66666666666666657415e-01,     /* 0x3FC55555, 0x55555555 */
pS1 = -3.25565818622400915405e-01,     /* 0xBFD4D612, 0x03EB6F7D */
pS2 =  2.01212532134862925881e-01,     /* 0x3FC9C155, 0x0E884455 */
pS3 = -4.00555345006794114027e-02,     /* 0xBFA48228, 0xB5688F3B */
pS4 =  7.91534994289814532176e-04,     /* 0x3F49EFE0, 0x7501B288 */
pS5 =  3.47933107596021167570e-05,     /* 0x3F023DE1, 0x0DFDF709 */
qS1 = -2.40339491173441421878e+00,     /* 0xC0033A27, 0x1C8A2D4B */
qS2 =  2.02094576023350569471e+00,     /* 0x40002AE5, 0x9C598AC8 */
qS3 = -6.88283971605453293030e-01,     /* 0xBFE6066C, 0x1B8D0159 */
qS4 =  7.70381505559019352791e-02;     /* 0x3FB3B8C5, 0xB12E9282 */

INLINE STATIC double
__acos(double x)
{
	double z,p,q,r,w,s,c,df;
	int hx,ix;

	GET_HIGH_WORD(hx,x);
	ix = hx&0x7fffffff;

	if(ix>=0x3ff00000)                  /* |x| >= 1 */
	{
	    unsigned int lx;
	    GET_LOW_WORD(lx,x);
	    if(((ix-0x3ff00000)|lx)==0) 	/* |x|==1 */
		{	
			if(hx>0) 
				return 0.0;		        /* acos(1) = 0  */
			else 
				return pi+2.0*pio2_lo;	/* acos(-1)= pi */
	    }
	    return (x-x)/(x-x);		        /* acos(|x|>1) is NaN */
	}
	if(ix<0x3fe00000)                   /* |x| < 0.5 */
	{	
	    if(ix<=0x3c600000) 
			return pio2_hi+pio2_lo;     /*if|x|<2**-57*/

	    z = x*x;
	    p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
	    q = one+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
	    r = p/q;
	    return pio2_hi - (x - (pio2_lo-x*r));
	} 
	else  if (hx<0) 		            /* x < -0.5 */
	{
	    z = (one+x)*0.5;
	    p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
	    q = one+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
	    s = sqrt(z);
	    r = p/q;
	    w = r*s-pio2_lo;
	    return pi - 2.0*(s+w);
	} 
	else 			                    /* x > 0.5 */
	{
	    z = (one-x)*0.5;
	    s = sqrt(z);
	    df = s;
	    SET_LOW_WORD(df,0);
	    c  = (z-df*df)/(s+df);
	    p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
	    q = one+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
	    r = p/q;
	    w = r*s+c;
	    return 2.0*(df+w);
	}
}

#endif /* PPC_FLOATING_POINT_SUPPORT */

/****************************************************************************/

double
acos(double x)
{
	double result;

	if(-1.0 <= x && x <= 1.0)
	{
		result = __acos(x);
	}
	else
	{
		result = 0;
		__set_errno(EDOM);
	}

	return(result);
}

/****************************************************************************/

#endif /* FLOATING_POINT_SUPPORT */
