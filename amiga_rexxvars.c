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

/* We don't want to pull in <clib/alib_protos.h> */
#define CLIB_ALIB_PROTOS_H

/****************************************************************************/

#ifndef _STDLIB_HEADERS_H
#include "stdlib_headers.h"
#endif /* _STDLIB_HEADERS_H */

/****************************************************************************/

#ifndef _STDLIB_CONSTRUCTOR_H
#include "stdlib_constructor.h"
#endif /* _STDLIB_CONSTRUCTOR_H */

/****************************************************************************/

#include <rexx/rxslib.h>
#include <rexx/errors.h>

/****************************************************************************/

#define __NOLIBBASE__
#include <proto/rexxsyslib.h>

/****************************************************************************/

/* This side-steps issues with very old inline header files, predating the
   OS 3.5 SDK, when the library is built for the 68k platform. */
#if defined(__GNUC__) && !defined(__amigaos4__)

#define __GetRexxVarFromMsg(name, buffer, message) ({ \
  STRPTR _GetRexxVarFromMsg_name = (name); \
  STRPTR _GetRexxVarFromMsg_buffer = (buffer); \
  struct RexxMsg * _GetRexxVarFromMsg_message = (message); \
  LONG _GetRexxVarFromMsg__re = \
  ({ \
  register struct RxsLib * const __GetRexxVarFromMsg__bn __asm("a6") = (struct RxsLib *) (RexxSysBase);\
  register LONG __GetRexxVarFromMsg__re __asm("d0"); \
  register STRPTR __GetRexxVarFromMsg_name __asm("a0") = (_GetRexxVarFromMsg_name); \
  register STRPTR __GetRexxVarFromMsg_buffer __asm("a1") = (_GetRexxVarFromMsg_buffer); \
  register struct RexxMsg * __GetRexxVarFromMsg_message __asm("a2") = (_GetRexxVarFromMsg_message); \
  __asm volatile ("jsr a6@(-492:W)" \
  : "=r"(__GetRexxVarFromMsg__re) \
  : "r"(__GetRexxVarFromMsg__bn), "r"(__GetRexxVarFromMsg_name), "r"(__GetRexxVarFromMsg_buffer), "r"(__GetRexxVarFromMsg_message) \
  : "d1", "a0", "a1", "fp0", "fp1", "cc", "memory"); \
  __GetRexxVarFromMsg__re; \
  }); \
  _GetRexxVarFromMsg__re; \
})

#undef GetRexxVarFromMsg
#define GetRexxVarFromMsg(name, buffer, message) __GetRexxVarFromMsg(name, buffer, message)

#define __SetRexxVarFromMsg(name, value, message) ({ \
  STRPTR _SetRexxVarFromMsg_name = (name); \
  STRPTR _SetRexxVarFromMsg_value = (value); \
  struct RexxMsg * _SetRexxVarFromMsg_message = (message); \
  LONG _SetRexxVarFromMsg__re = \
  ({ \
  register struct RxsLib * const __SetRexxVarFromMsg__bn __asm("a6") = (struct RxsLib *) (RexxSysBase);\
  register LONG __SetRexxVarFromMsg__re __asm("d0"); \
  register STRPTR __SetRexxVarFromMsg_name __asm("a0") = (_SetRexxVarFromMsg_name); \
  register STRPTR __SetRexxVarFromMsg_value __asm("a1") = (_SetRexxVarFromMsg_value); \
  register struct RexxMsg * __SetRexxVarFromMsg_message __asm("a2") = (_SetRexxVarFromMsg_message); \
  __asm volatile ("jsr a6@(-498:W)" \
  : "=r"(__SetRexxVarFromMsg__re) \
  : "r"(__SetRexxVarFromMsg__bn), "r"(__SetRexxVarFromMsg_name), "r"(__SetRexxVarFromMsg_value), "r"(__SetRexxVarFromMsg_message) \
  : "d1", "a0", "a1", "fp0", "fp1", "cc", "memory"); \
  __SetRexxVarFromMsg__re; \
  }); \
  _SetRexxVarFromMsg__re; \
})

#undef SetRexxVarFromMsg
#define SetRexxVarFromMsg(name, value, message) __SetRexxVarFromMsg(name, value, message)

#endif /* __GNUC__ && !__amigaos4__ */

/****************************************************************************/

static struct Library * RexxSysBase;

/****************************************************************************/

#if defined(__amigaos4__)
static struct RexxSysIFace * IRexxSys;
#endif /* __amigaos4__ */

/****************************************************************************/

BOOL CheckRexxMsg(struct RexxMsg *message);
LONG GetRexxVar(struct RexxMsg *message,STRPTR variable_name,STRPTR *buffer_pointer);
LONG SetRexxVar(struct RexxMsg *message,STRPTR variable_name,STRPTR value,ULONG length);

/****************************************************************************/

CLIB_CONSTRUCTOR(rexxvars_init)
{
	ENTER();

	RexxSysBase = OpenLibrary(RXSNAME,0);

	#if defined(__amigaos4__)
	{
		if(RexxSysBase != NULL)
		{
			IRexxSys = (struct RexxSysIFace *)GetInterface(RexxSysBase, "main", 1, 0);
			if(IRexxSys == NULL)
			{
				CloseLibrary(RexxSysBase);
				RexxSysBase = NULL;
			}
		}
	}
	#endif /* __amigaos4__ */

	LEAVE();

	CONSTRUCTOR_SUCCEED();
}

/****************************************************************************/

CLIB_DESTRUCTOR(rexxvars_exit)
{
	ENTER();

	#if defined(__amigaos4__)
	{
		if(IRexxSys != NULL)
			DropInterface((struct Interface *)IRexxSys);

		IRexxSys = NULL;
	}
	#endif /* __amigaos4__ */

	if(RexxSysBase != NULL)
	{
		CloseLibrary(RexxSysBase);
		RexxSysBase = NULL;
	}

	LEAVE();
}

/****************************************************************************/

/* This is modeled after the original assembly language code. Except for the
   fact that we compare the library base against a local, static variable
   rather then opening the library for each check. */
BOOL
CheckRexxMsg(struct RexxMsg *message)
{
	BOOL result = FALSE;

	if(RexxSysBase == NULL)
		goto out;

	if(message->rm_LibBase != RexxSysBase)
		goto out;

	if(message->rm_TaskBlock == NULL)
		goto out;

	if(NOT IsRexxMsg(message))
		goto out;

	result = TRUE;

 out:

	return(result);
}

/****************************************************************************/

/* The following function works in about like the original, except that it's
   not reentrant, does not fill in a pointer to the variable itself and
   requires rexxsyslib.library V45. */
LONG
GetRexxVar(struct RexxMsg *message,STRPTR variable_name,STRPTR *buffer_pointer)
{
	static TEXT buffer[256];
	LONG result;

	/* The following uses a function which was added to rexxsyslib.library V45.
	   We therefore have a minimum library version requirement. */
	if(RexxSysBase == NULL || RexxSysBase->lib_Version < 45 || NOT IsRexxMsg(message))
	{
		result = ERR10_010; /* invalid message packet */
		goto out;
	}

	/* The 256 character limit isn't good. This should be done differently. */
	result = GetRexxVarFromMsg(variable_name,buffer,message);
	if(result != 0)
		goto out;

	(*buffer_pointer) = buffer;

 out:

	return(result);
}

/****************************************************************************/

/* The following function works in about like the original, except that it
   ignores the length parameter (the value needs to be a NUL-terminated string)
   and requires rexxsyslib.library V45. */
LONG
SetRexxVar(struct RexxMsg *message,STRPTR variable_name,STRPTR value,ULONG length)
{
	LONG result;

	/* The following uses a function which was added to rexxsyslib.library V45.
	   We therefore have a minimum library version requirement. */
	if(RexxSysBase == NULL || RexxSysBase->lib_Version < 45 || NOT IsRexxMsg(message))
	{
		result = ERR10_010; /* invalid message packet */
		goto out;
	}

	result = SetRexxVarFromMsg(variable_name,value,message);

 out:

	return(result);
}
