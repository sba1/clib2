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

#ifndef _STDLIB_HEADERS_H
#include "stdlib_headers.h"
#endif /* _STDLIB_HEADERS_H */

/****************************************************************************/

void
__vasprintf_hook_entry(
	struct Hook *				UNUSED	unused_hook,
	struct iob *						string_iob,
	struct file_hook_message *			message)
{
	int result = -1;
	int error = OK;
	int num_bytes_left;
	int num_bytes;

	assert( message != NULL && string_iob != NULL );
	assert( message->action == file_hook_action_write );

	if(message->action != file_hook_action_write)
	{
		error = EBADF;
		goto out;
	}

	if(string_iob->iob_StringPosition + message->size > string_iob->iob_StringSize)
	{
		const int granularity = 64;

		size_t new_size;
		char * buffer;

		new_size = string_iob->iob_StringPosition + message->size + granularity;

		buffer = __malloc(new_size,string_iob->iob_File,string_iob->iob_Line);
		if(buffer == NULL)
		{
			error = ENOBUFS;
			goto out;
		}

		if(string_iob->iob_String != NULL)
		{
			memmove(buffer,string_iob->iob_String,(size_t)string_iob->iob_StringSize);

			__free(string_iob->iob_String,string_iob->iob_File,string_iob->iob_Line);
		}

		string_iob->iob_String		= buffer;
		string_iob->iob_StringSize	= new_size;
	}

	assert( string_iob->iob_StringPosition <= string_iob->iob_StringSize );

	num_bytes_left = string_iob->iob_StringSize - string_iob->iob_StringPosition;

	num_bytes = message->size;
	if(num_bytes > num_bytes_left)
		num_bytes = num_bytes_left;

	assert( num_bytes >= 0 );
	assert( message->data != NULL );

	memmove(&string_iob->iob_String[string_iob->iob_StringPosition],message->data,(size_t)num_bytes);
	string_iob->iob_StringPosition += num_bytes;

	result = num_bytes;

 out:

	message->result	= result;
	message->error	= error;
}
