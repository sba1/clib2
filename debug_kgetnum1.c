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

#include "debug_headers.h"

/****************************************************************************/

LONG
kgetnum(VOID)
{
	LONG sum = 0;
	LONG result;
	BOOL is_negative = FALSE;
	LONG position = 0;
	LONG c;

	while(TRUE)
	{
		c = kgetc();

		if(c == '-')
		{
			if(position == 0)
			{
				is_negative = TRUE;

				kputc(c);

				position++;
			}
		}
		else if (c == '\b')
		{
			if(position > 0)
			{
				position--;
				if(position == 0)
				{
					is_negative = FALSE;

					sum = 0;
				}
				else
				{
					sum = sum / 10;
				}

				kputc(c);
				kputc(' ');
				kputc(c);
			}
		}
		else if (c == '\r')
		{
			kputc('\n');
			break;
		}
		else if ('0' <= c && c <= '9')
		{
			LONG new_sum;

			new_sum = sum * 10 + (c - '0');
			if(new_sum >= sum)
			{
				sum = new_sum;

				kputc(c);

				position++;
			}
		}
	}

	if(is_negative)
		result = (-sum);
	else
		result = sum;

	return(result);
}
