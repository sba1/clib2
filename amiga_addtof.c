/*
 * $Id$
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2005 by Olaf Barthel <olsen@sourcery.han.de>
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

#include <exec/interrupts.h>
#include <hardware/intbits.h>
#include <graphics/graphint.h>

#include <string.h>

/****************************************************************************/

#include <proto/exec.h>
#include <proto/graphics.h>
#include <clib/alib_protos.h>

/****************************************************************************/

#include "macros.h"

/****************************************************************************/

#include "debug.h"

/****************************************************************************/

typedef LONG (* CFUNC)(APTR arg);

/****************************************************************************/

STATIC LONG INTERRUPT ASM
call_routine(REG(a1,struct Isrvstr *i))
{
	CFUNC p = (CFUNC)i->ccode;

	(*p)(i->Carg);

	return(0);
}

/****************************************************************************/

VOID
AddTOF(struct Isrvstr *i,CFUNC p,APTR arg)
{
	assert( i != NULL && p != NULL );

	i->code		= (long (*)())call_routine;
	i->Iptr		= i;
	i->ccode	= p;
	i->Carg		= arg;

	AddIntServer(INTB_VERTB,(struct Interrupt *)i);
}
