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

#ifndef _LOCALE_HEADERS_H
#include "locale_headers.h"
#endif /* _LOCALE_HEADERS_H */

/****************************************************************************/

#ifndef _STDLIB_CONSTRUCTOR_H
#include "stdlib_constructor.h"
#endif /* _STDLIB_CONSTRUCTOR_H */

/****************************************************************************/

struct Library * NOCOMMON __LocaleBase;

/****************************************************************************/

#if defined(__amigaos4__)
struct LocaleIFace * NOCOMMON __ILocale;
#endif /* __amigaos4__ */

/****************************************************************************/

struct Locale * NOCOMMON __default_locale;
struct Locale * NOCOMMON __locale_table[NUM_LOCALES];

/****************************************************************************/

char NOCOMMON __locale_name_table[NUM_LOCALES][MAX_LOCALE_NAME_LEN];

/****************************************************************************/

void
__close_all_locales(void)
{
	__locale_lock();

	if(__LocaleBase != NULL)
	{
		DECLARE_LOCALEBASE();

		int i;

		for(i = 0 ; i < NUM_LOCALES ; i++)
		{
			if(i == LC_ALL)
				continue;

			if(__locale_table[i] != NULL)
			{
				if(__locale_table[i] != __locale_table[LC_ALL])
					CloseLocale(__locale_table[i]);

				__locale_table[i] = NULL;
			}
		}

		CloseLocale(__locale_table[LC_ALL]);
		__locale_table[LC_ALL] = NULL;
	}

	__locale_unlock();
}

/****************************************************************************/

void
__locale_exit(void)
{
	ENTER();

	__locale_lock();

	if(__LocaleBase != NULL)
	{
		DECLARE_LOCALEBASE();

		__close_all_locales();

		if(__default_locale != NULL)
		{
			CloseLocale(__default_locale);
			__default_locale = NULL;
		}

		#if defined(__amigaos4__)
		{
			if(__ILocale != NULL)
			{
				DropInterface((struct Interface *)__ILocale);
				__ILocale = NULL;
			}
		}
		#endif /* __amigaos4__ */

		CloseLibrary(__LocaleBase);
		__LocaleBase = NULL;
	}

	__locale_unlock();

	LEAVE();
}

/****************************************************************************/

int
__locale_init(void)
{
	int result = ERROR;

	ENTER();

	PROFILE_OFF();

	__locale_lock();

	if(__LocaleBase == NULL)
	{
		__LocaleBase = OpenLibrary("locale.library",38);

		#if defined(__amigaos4__)
		{
			if (__LocaleBase != NULL)
			{
				__ILocale = (struct LocaleIFace *)GetInterface(__LocaleBase, "main", 1, 0);
				if(__ILocale == NULL)
				{
					CloseLibrary(__LocaleBase);
					__LocaleBase = NULL;
				}
			}
		}
		#endif /* __amigaos4__ */
	}

	if(__LocaleBase != NULL && __default_locale == NULL)
	{
		DECLARE_LOCALEBASE();

		__default_locale = OpenLocale(NULL);
	}

	if(__default_locale != NULL)
		result = OK;

	__locale_unlock();

	PROFILE_ON();

	RETURN(result);
	return(result);
}

/****************************************************************************/

#if defined(__THREAD_SAFE)

/****************************************************************************/

static struct SignalSemaphore * locale_lock;

/****************************************************************************/

void
__locale_lock(void)
{
	if(locale_lock != NULL)
		ObtainSemaphore(locale_lock);
}

/****************************************************************************/

void
__locale_unlock(void)
{
	if(locale_lock != NULL)
		ReleaseSemaphore(locale_lock);
}

/****************************************************************************/

#endif /* __THREAD_SAFE */

/****************************************************************************/

CLIB_DESTRUCTOR(locale_exit)
{
	ENTER();

	__locale_exit();

	#if defined(__THREAD_SAFE)
	{
		__delete_semaphore(locale_lock);
		locale_lock = NULL;
	}
	#endif /* __THREAD_SAFE */

	LEAVE();
}

/****************************************************************************/

CLIB_CONSTRUCTOR(locale_init)
{
	BOOL success = FALSE;
	int i;

	ENTER();

	#if defined(__THREAD_SAFE)
	{
		locale_lock = __create_semaphore();
		if(locale_lock == NULL)
			goto out;
	}
	#endif /* __THREAD_SAFE */

	for(i = 0 ; i < NUM_LOCALES ; i++)
		strcpy(__locale_name_table[i],"C");

	if(__open_locale)
		__locale_init();

	success = TRUE;

 out:

	SHOWVALUE(success);
	LEAVE();

	if(success)
		CONSTRUCTOR_SUCCEED();
	else
		CONSTRUCTOR_FAIL();
}
