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
 */

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

/****************************************************************************/

#ifndef _STDLIB_CONSTRUCTOR_H
#include "stdlib_constructor.h"
#endif /* _STDLIB_CONSTRUCTOR_H */

/****************************************************************************/

#ifndef EXEC_EXECBASE_H
#include <exec/execbase.h>
#endif /* EXEC_EXECBASE_H */

/****************************************************************************/

#ifndef PROTO_EXEC_H
#include <proto/exec.h>
#endif /* PROTO_EXEC_H */

/****************************************************************************/

#include <string.h>

/****************************************************************************/

#if defined(FLOATING_POINT_SUPPORT)

/****************************************************************************/

#if defined(IEEE_FLOATING_POINT_SUPPORT)

struct Library * NOCOMMON MathIeeeSingBasBase;
struct Library * NOCOMMON MathIeeeDoubBasBase;
struct Library * NOCOMMON MathIeeeDoubTransBase;

#endif /* IEEE_FLOATING_POINT_SUPPORT */

/****************************************************************************/

float	NOCOMMON __infinity;
float	NOCOMMON __nan;

/****************************************************************************/

float	NOCOMMON __huge_val_float;
double	NOCOMMON __huge_val;

/****************************************************************************/

#if defined(IEEE_FLOATING_POINT_SUPPORT)

/****************************************************************************/

MATH_DESTRUCTOR(math_exit)
{
	ENTER();

	if(MathIeeeSingBasBase != NULL)
	{
		CloseLibrary(MathIeeeSingBasBase);
		MathIeeeSingBasBase = NULL;
	}

	if(MathIeeeDoubBasBase != NULL)
	{
		CloseLibrary(MathIeeeDoubBasBase);
		MathIeeeDoubBasBase = NULL;
	}

	if(MathIeeeDoubTransBase != NULL)
	{
		CloseLibrary(MathIeeeDoubTransBase);
		MathIeeeDoubTransBase = NULL;
	}

	LEAVE();
}

/****************************************************************************/

#endif /* IEEE_FLOATING_POINT_SUPPORT */

/****************************************************************************/

MATH_CONSTRUCTOR(math_init)
{
	union ieee_double * double_x;
	union ieee_single * single_x;
	BOOL success = FALSE;

	ENTER();

	#if defined(M68881_FLOATING_POINT_SUPPORT)
	{
		if(FLAG_IS_CLEAR(((struct ExecBase *)SysBase)->AttnFlags,AFF_68881))
		{
			__show_error("This program requires a floating point processor.");

			goto out;
		}
	}
	#endif /* M68881_FLOATING_POINT_SUPPORT */

	#if defined(IEEE_FLOATING_POINT_SUPPORT)
	{
		char * failed_library = NULL;

		PROFILE_OFF();

		/* Try to get the soft floating point libraries open. */
		MathIeeeSingBasBase		= OpenLibrary("mathieeesingbas.library",33);
		MathIeeeDoubBasBase		= OpenLibrary("mathieeedoubbas.library",33);
		MathIeeeDoubTransBase	= OpenLibrary("mathieeedoubtrans.library",33);

		PROFILE_ON();

		if(MathIeeeSingBasBase == NULL)
			failed_library = "mathieeesingbas.library";
		else if (MathIeeeDoubBasBase == NULL)
			failed_library = "mathieeedoubbas.library";
		else if (MathIeeeDoubTransBase == NULL)
			failed_library = "mathieeedoubtrans.library";

		if(failed_library != NULL)
		{
			char message[60];

			strlcpy(message,failed_library,sizeof(message));
			strlcat(message," could not be opened.",sizeof(message));

			__show_error(message);

			goto out;
		}
	}
	#endif /* IEEE_FLOATING_POINT_SUPPORT */

	/* Now fill in HUGE_VAL and HUGE_VALF, respectively. TODO:
	   also take care of HUGE_VALL. */

	/* Exponent = +126, Mantissa = 8,388,607 */
	single_x = (union ieee_single *)&__huge_val_float;
	single_x->raw[0] = 0x7f7fffff;

	/* Exponent = +1022, Mantissa = 4,503,599,627,370,495 */
	double_x = (union ieee_double *)&__huge_val;
	double_x->raw[0] = 0x7fefffff;
	double_x->raw[1] = 0xffffffff;

	#if defined(USE_LONG_DOUBLE)
	{
		union ieee_long_double * x = (union ieee_long_double *)&__huge_val_long_double;

		/* Exponent = +32766, Mantissa = 18,446,744,073,709,551,615 */
		x->raw[0] = 0x7ffe0000;
		x->raw[1] = 0xffffffff;
		x->raw[2] = 0xffffffff;
	}
	#endif /* USE_LONG_DOUBLE */

	/* Finally, fill in the constants behind INFINITY and NAN. */
	single_x = (union ieee_single *)&__infinity;
	single_x->raw[0] = 0x7f800000;

	single_x = (union ieee_single *)&__nan;
	single_x->raw[0] = 0x7fc00001;

	success = TRUE;

 out:

	SHOWVALUE(success);
	LEAVE();

	if(success)
		CONSTRUCTOR_SUCCEED();
	else
		CONSTRUCTOR_FAIL();
}

/****************************************************************************/

#endif /* FLOATING_POINT_SUPPORT */
