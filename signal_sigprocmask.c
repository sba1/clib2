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

#ifndef _SIGNAL_HEADERS_H
#include "signal_headers.h"
#endif /* _SIGNAL_HEADERS_H */

/****************************************************************************/

/* The following is not part of the ISO 'C' (1994) standard. */

/****************************************************************************/

int
sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	int result = -1;

	ENTER();

	SHOWVALUE(how);
	SHOWPOINTER(set);
	SHOWPOINTER(oset);

	if(how < SIG_BLOCK || how > SIG_SETMASK)
	{
		SHOWMSG("invalid operating mode");

		errno = EINVAL;
		goto out;
	}

	if(oset != NULL)
		(*oset) = __signals_blocked;

	if(set != NULL)
	{
		SHOWVALUE(*set);

		switch(how)
		{
			case SIG_BLOCK:

				SHOWMSG("SIG_BLOCK");

				__signals_blocked |= (*set);
				break;

			case SIG_UNBLOCK:

				SHOWMSG("SIG_UNBLOCK");

				__signals_blocked &= ~(*set);
				break;

			case SIG_SETMASK:

				SHOWMSG("SIG_SETMASK");

				__signals_blocked = (*set);
				break;
		}
	}

	result = 0;

 out:

	RETURN(result);
	return(result);
}
