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

#ifndef _STAT_HEADERS_H
#include "stat_headers.h"
#endif /* _STAT_HEADERS_H */

/****************************************************************************/

/* The following is not part of the ISO 'C' (1994) standard. */

/****************************************************************************/

int
fchmod(int file_descriptor, mode_t mode)
{
	D_S(struct FileInfoBlock,fib);
	ULONG protection;
	BPTR parent_dir = ZERO;
	BPTR old_current_dir = ZERO;
	BOOL current_dir_changed = FALSE;
	int result = -1;
	struct fd * fd = NULL;
	LONG success;

	ENTER();

	SHOWVALUE(file_descriptor);
	SHOWVALUE(mode);

	assert( file_descriptor >= 0 && file_descriptor < __num_fd );
	assert( __fd[file_descriptor] != NULL );
	assert( FLAG_IS_SET(__fd[file_descriptor]->fd_Flags,FDF_IN_USE) );

	if(__check_abort_enabled)
		__check_abort();

	fd = __get_file_descriptor(file_descriptor);
	if(fd == NULL)
	{
		__set_errno(EBADF);
		goto out;
	}

	__fd_lock(fd);

	if(FLAG_IS_SET(fd->fd_Flags,FDF_IS_SOCKET))
	{
		__set_errno(EINVAL);
		goto out;
	}

	if(FLAG_IS_SET(fd->fd_Flags,FDF_STDIO))
	{
		__set_errno(EBADF);
		goto out;
	}

	protection = 0;

	if(FLAG_IS_SET(mode,S_IRUSR))
		SET_FLAG(protection,FIBF_READ);

	if(FLAG_IS_SET(mode,S_IWUSR))
	{
		SET_FLAG(protection,FIBF_WRITE);
		SET_FLAG(protection,FIBF_DELETE);
	}

	if(FLAG_IS_SET(mode,S_IXUSR))
		SET_FLAG(protection,FIBF_EXECUTE);

	if(FLAG_IS_SET(mode,S_IRGRP))
		SET_FLAG(protection,FIBF_GRP_READ);

	if(FLAG_IS_SET(mode,S_IWGRP))
	{
		SET_FLAG(protection,FIBF_GRP_WRITE);
		SET_FLAG(protection,FIBF_GRP_DELETE);
	}

	if(FLAG_IS_SET(mode,S_IXGRP))
		SET_FLAG(protection,FIBF_GRP_EXECUTE);

	if(FLAG_IS_SET(mode,S_IROTH))
		SET_FLAG(protection,FIBF_OTR_READ);

	if(FLAG_IS_SET(mode,S_IWOTH))
	{
		SET_FLAG(protection,FIBF_OTR_WRITE);
		SET_FLAG(protection,FIBF_OTR_DELETE);
	}

	if(FLAG_IS_SET(mode,S_IXOTH))
		SET_FLAG(protection,FIBF_OTR_EXECUTE);

	PROFILE_OFF();
	parent_dir = __safe_parent_of_file_handle(fd->fd_DefaultFile);
	PROFILE_ON();

	if(parent_dir == ZERO)
	{
		SHOWMSG("couldn't find parent directory");

		__set_errno(__translate_io_error_to_errno(IoErr()));
		goto out;
	}

	PROFILE_OFF();
	success = __safe_examine_file_handle(fd->fd_DefaultFile,fib);
	PROFILE_ON();

	if(NO success)
	{
		SHOWMSG("could not obtain file name");

		__set_errno(__translate_io_error_to_errno(IoErr()));
		goto out;
	}

	old_current_dir = CurrentDir(parent_dir);
	current_dir_changed = TRUE;

	PROFILE_OFF();
	success = SetProtection(fib->fib_FileName,protection ^ (FIBF_READ|FIBF_WRITE|FIBF_EXECUTE|FIBF_DELETE));
	PROFILE_ON();

	if(NO success)
	{
		SHOWMSG("could not change protection bits");

		__set_errno(__translate_io_error_to_errno(IoErr()));
		goto out;
	}

	result = 0;

 out:

	__fd_unlock(fd);

	if(current_dir_changed)
		CurrentDir(old_current_dir);

	UnLock(parent_dir);

	RETURN(result);
	return(result);
}
