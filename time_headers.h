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

#ifndef _TIME_HEADERS_H
#define _TIME_HEADERS_H

/****************************************************************************/

#include <exec/libraries.h>
#include <exec/devices.h>
#include <exec/memory.h>

#include <dos/dosextens.h>

#include <devices/timer.h>

#include <libraries/locale.h>

#include <utility/date.h>

/****************************************************************************/

#include <clib/alib_protos.h>

/****************************************************************************/

#include <proto/exec.h>
#include <proto/dos.h>

/****************************************************************************/

#include <string.h>
#include <locale.h>
#include <errno.h>
#include <time.h>
#include <dos.h>

/****************************************************************************/

#ifndef _STDLIB_LOCALE_UTILITY_H
#include "stdlib_locale_utility.h"
#endif /* _STDLIB_LOCALE_UTILITY_H */

/****************************************************************************/

#include "macros.h"
#include "debug.h"

/****************************************************************************/

#ifndef _MATH_FP_SUPPORT_H
#include "math_fp_support.h"
#endif /* _MATH_FP_SUPPORT_H */

/****************************************************************************/

extern struct DateStamp __start_time;

/****************************************************************************/

extern const char * const __abbreviated_week_day_names[7];
extern const char * const __week_day_names[7];
extern const char * const __abbreviated_month_names[12];
extern const char * const __month_names[12];

/****************************************************************************/

extern char * __number_to_string(unsigned int number,char * string,size_t max_len,size_t min_len);
extern struct tm * __convert_time(ULONG seconds, LONG gmt_offset, struct tm * tm);

/****************************************************************************/

#endif /* _TIME_HEADERS_H */
