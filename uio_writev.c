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

#ifndef _UIO_HEADERS_H
#include "uio_headers.h"
#endif /* _UIO_HEADERS_H */

/****************************************************************************/

/* The following is not part of the ISO 'C' (1994) standard. */

/****************************************************************************/

ssize_t
writev(int file_descriptor,const struct iovec *iov,int vec_count)
{
	ssize_t result = -1;
	struct file_action_message msg;
	ssize_t total_num_bytes_written;
	char * buffer = NULL;
	struct fd * fd;
	int i;

	ENTER();

	SHOWVALUE(file_descriptor);
	SHOWPOINTER(iov);
	SHOWVALUE(vec_count);

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(iov == NULL)
		{
			__set_errno(EFAULT);
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	if(vec_count < 1 || vec_count > MAX_IOVEC)
	{
		__set_errno(EINVAL);
		goto out;
	}

	/* Check for overflow. An expensive test, but better to do it
	   here than in the write loop. */
	for(i = 0, total_num_bytes_written = 0 ; i < vec_count ; i++)
	{
		if(iov[i].iov_len < 0)
		{
			/* Paraoia... */
			__set_errno(EINVAL);
			goto out;
		}

		total_num_bytes_written += iov[i].iov_len;
		if(total_num_bytes_written < 0) /* Rollover. */
		{
			__set_errno(EINVAL);
			goto out;
		}
	}

	fd = __get_file_descriptor(file_descriptor);
	if(fd == NULL)
	{
		__set_errno(EBADF);
		goto out;
	}

	buffer = malloc(total_num_bytes_written);
	if(buffer != NULL)
	{
		char * b = buffer;

		for(i = 0 ; i < vec_count ; i++)
		{
			memcpy(b,iov[i].iov_base,iov[i].iov_len);
			b += iov[i].iov_len;
		}

		msg.fam_Action	= file_action_write;
		msg.fam_Data	= buffer;
		msg.fam_Size	= total_num_bytes_written;

		if((*fd->fd_Action)(fd,&msg) < 0)
		{
			__set_errno(msg.fam_Error);
			goto out;
		}
	}
	else
	{
		for(i = 0 ; i < vec_count ; i++)
		{
			if(__check_abort_enabled)
				__check_abort();

			if(iov[i].iov_len > 0)
			{
				msg.fam_Action	= file_action_write;
				msg.fam_Data	= (char *)iov[i].iov_base;
				msg.fam_Size	= iov[i].iov_len;

				if((*fd->fd_Action)(fd,&msg) < 0)
				{
					__set_errno(msg.fam_Error);
					goto out;
				}
			}
		}
	}

	result = total_num_bytes_written;

out:

	if(buffer != NULL)
		free(buffer);

	RETURN(result);
	return(result);
}
