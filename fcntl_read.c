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

#ifndef _STDLIB_NULL_POINTER_CHECK_H
#include "stdlib_null_pointer_check.h"
#endif /* _STDLIB_NULL_POINTER_CHECK_H */

/****************************************************************************/

#ifndef _FCNTL_HEADERS_H
#include "fcntl_headers.h"
#endif /* _FCNTL_HEADERS_H */

/****************************************************************************/

/* The following is not part of the ISO 'C' (1994) standard. */

/****************************************************************************/

ssize_t
read(int file_descriptor, void * buffer, size_t num_bytes)
{
	ssize_t num_bytes_read;
	struct fd * fd = NULL;
	ssize_t result = EOF;

	ENTER();

	SHOWVALUE(file_descriptor);
	SHOWPOINTER(buffer);
	SHOWVALUE(num_bytes);

	assert( buffer != NULL );
	assert( (int)num_bytes >= 0);

	if(__check_abort_enabled)
		__check_abort();

	__stdio_lock();

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(buffer == NULL)
		{
			SHOWMSG("invalid buffer");

			__set_errno(EFAULT);
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	assert( file_descriptor >= 0 && file_descriptor < __num_fd );
	assert( __fd[file_descriptor] != NULL );
	assert( FLAG_IS_SET(__fd[file_descriptor]->fd_Flags,FDF_IN_USE) );

	fd = __get_file_descriptor(file_descriptor);
	if(fd == NULL)
	{
		__set_errno(EBADF);
		goto out;
	}

	__fd_lock(fd);

	if(FLAG_IS_CLEAR(fd->fd_Flags,FDF_READ))
	{
		SHOWMSG("this descriptor is not read-enabled");

		__set_errno(EBADF);
		goto out;
	}

	if(num_bytes > 0)
	{
		struct file_action_message fam;

		SHOWMSG("calling the hook");

		fam.fam_Action	= file_action_read;
		fam.fam_Data	= buffer;
		fam.fam_Size	= num_bytes;

		assert( fd->fd_Action != NULL );

		num_bytes_read = (*fd->fd_Action)(fd,&fam);
		if(num_bytes_read == EOF)
		{
			__set_errno(fam.fam_Error);
			goto out;
		}
	}
	else
	{
		num_bytes_read = 0;
	}

	result = num_bytes_read;

 out:

	__fd_unlock(fd);

	__stdio_unlock();

	RETURN(result);
	return(result);
}
