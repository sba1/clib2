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

#ifndef _STDLIB_NULL_POINTER_CHECK_H
#include "stdlib_null_pointer_check.h"
#endif /* _STDLIB_NULL_POINTER_CHECK_H */

/****************************************************************************/

#ifndef _STDIO_HEADERS_H
#include "stdio_headers.h"
#endif /* _STDIO_HEADERS_H */

/****************************************************************************/

char *
gets(char *s)
{
	char * result = s;
	int c;

	ENTER();

	SHOWPOINTER(s);

	assert( s != NULL && stdin != NULL );

	if(__check_abort_enabled)
		__check_abort();

	flockfile(stdin);

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(s == NULL || stdin == NULL)
		{
			SHOWMSG("invalid parameters");

			__set_errno(EFAULT);

			result = NULL;
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	/* Take care of the checks and data structure changes that
	 * need to be handled only once for this stream.
	 */
	if(__fgetc_check(stdin) < 0)
	{
		result = NULL;
		goto out;
	}

	/* So that we can tell error and 'end of file' conditions apart. */
	clearerr(stdin);

	while(TRUE)
	{
		c = __getc(stdin);
		if(c == EOF)
		{
			if(ferror(stdin))
			{
				/* Just to be on the safe side. */
				(*s) = '\0';

				result = NULL;
				goto out;
			}

			/* Make sure that we return NULL if we really
			   didn't read anything at all */
			if(s == result)
				result = NULL;

			break;
		}

		if(c == '\n')
			break;

		(*s++) = c;
	}

	(*s) = '\0';

	SHOWSTRING(result);

 out:

	funlockfile(stdin);

	RETURN(result);
	return(result);
}
