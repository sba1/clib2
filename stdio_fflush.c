/*
 * $Id$
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2004 by Olaf Barthel <olsen@sourcery.han.de>
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

int
fflush(FILE *stream)
{
	int result = EOF;

	ENTER();

	SHOWPOINTER(stream);

	if(__check_abort_enabled)
		__check_abort();

	#if defined(UNIX_PATH_SEMANTICS)
	{
		/* Flush a particular stream? */
		if(stream != NULL)
		{
			struct iob * file = (struct iob *)stream;

			assert( __is_valid_iob(file) );

			if(__iob_write_buffer_is_valid(file) && __flush_iob_write_buffer(file) < 0)
				goto out;
		}
		else
		{
			int i;

			/* Flush all streams which still have unwritten data in the buffer. */
			for(i = 0 ; i < __num_iob ; i++)
			{
				if(__iob[i] != NULL &&
				   FLAG_IS_SET(__iob[i]->iob_Flags,IOBF_IN_USE) &&
				   FLAG_IS_SET(__iob[i]->iob_Flags,IOBF_WRITE) &&
				   __iob_write_buffer_is_valid(__iob[i]))
				{
					if(__flush_iob_write_buffer(__iob[i]) < 0)
						goto out;
				}
			}
		}
	}
	#else
	{
		struct iob * file = (struct iob *)stream;

		assert( stream != NULL );

		#if defined(CHECK_FOR_NULL_POINTERS)
		{
			if(stream == NULL)
			{
				SHOWMSG("invalid file handle");

				errno = EFAULT;
				goto out;
			}
		}
		#endif /* CHECK_FOR_NULL_POINTERS */

		assert( __is_valid_iob(file) );

		if(__iob_write_buffer_is_valid(file) && __flush_iob_write_buffer(file) < 0)
			goto out;
	}
	#endif /* UNIX_PATH_SEMANTICS */

	result = OK;

 out:

	RETURN(result);
	return(result);
}
