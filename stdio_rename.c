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

#ifndef _STDIO_HEADERS_H
#include "stdio_headers.h"
#endif /* _STDIO_HEADERS_H */

/****************************************************************************/

int
rename(const char *oldname,const char *newname)
{
	#if defined(UNIX_PATH_SEMANTICS)
	struct name_translation_info old_nti;
	struct name_translation_info new_nti;
	#endif /* UNIX_PATH_SEMANTICS */
	int result = ERROR;
	LONG status;

	ENTER();

	SHOWSTRING(oldname);
	SHOWSTRING(newname);

	assert( oldname != NULL && newname != NULL );

	if(__check_abort_enabled)
		__check_abort();

	PROFILE_OFF();

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(oldname == NULL || newname == NULL)
		{
			SHOWMSG("invalid parameters");

			__set_errno(EFAULT);
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	#if defined(UNIX_PATH_SEMANTICS)
	{
		if(__unix_path_semantics)
		{
			if(oldname[0] == '\0' || newname[0] == '\0')
			{
				SHOWMSG("no name given");

				__set_errno(ENOENT);
				goto out;
			}

			if(__translate_unix_to_amiga_path_name(&oldname,&old_nti) != 0)
				goto out;

			if(__translate_unix_to_amiga_path_name(&newname,&new_nti) != 0)
				goto out;

			if(old_nti.is_root || new_nti.is_root)
			{
				__set_errno(EACCES);
				goto out;
			}
		}
	}
	#endif /* UNIX_PATH_SEMANTICS */

	D(("renaming '%s' to '%s'",oldname,newname));

	status = Rename((STRPTR)oldname,(STRPTR)newname);

	if(status == DOSFALSE)
	{
		SHOWMSG("that didn't work");

		#if defined(UNIX_PATH_SEMANTICS)
		{
			LONG error;

			error = IoErr();
			if(error != ERROR_OBJECT_EXISTS)
			{
				SHOWMSG("that was some other error");

				__set_errno(__translate_io_error_to_errno(error));
				goto out;
			}

			SHOWMSG("deleting the target file and renaming the source file");

			/* ZZZ there should be a safer solution for this */

			if(CANNOT DeleteFile((STRPTR)newname))
			{
				SHOWMSG("couldn't delete the file");

				__set_errno(__translate_io_error_to_errno(error));
				goto out;
			}

			if(CANNOT Rename((STRPTR)oldname,(STRPTR)newname))
			{
				SHOWMSG("that didn't work");

				__set_errno(__translate_access_io_error_to_errno(IoErr()));
				goto out;
			}
		}
		#else
		{
			__set_errno(__translate_io_error_to_errno(IoErr()));
			goto out;
		}
		#endif /* UNIX_PATH_SEMANTICS */
	}

	result = OK;

 out:

	PROFILE_ON();

	RETURN(result);
	return(result);
}
