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

#ifndef _TIME_HEADERS_H
#include "time_headers.h"
#endif /* _TIME_HEADERS_H */

#ifndef _LOCALE_HEADERS_H
#include "locale_headers.h"
#endif /* _LOCALE_HEADERS_H */

/****************************************************************************/

struct format_hook_data
{
	char *	buffer;
	int		max_size;
	int		len;
};

/****************************************************************************/

STATIC VOID
format_hook_function(
	struct Hook *			hook,
	long					c,
	struct Locale *	UNUSED	unused_locale)
{
	struct format_hook_data * data = hook->h_Data;

	if(c != '\0')
	{
		if(data->max_size > 0)
		{
			/* There's still room for another character. */
			(*data->buffer++) = (char)c;

			data->len++;

			data->max_size--;
		}
		else
		{
			/* Indicate that the buffer was too short. */
			data->len = 0;
		}
	}
}

/****************************************************************************/

STATIC VOID
store_string_via_hook(const char * string,int len,struct Hook * hook)
{
	DECLARE_UTILITYBASE();

	assert( string != NULL && hook != NULL );
	assert( UtilityBase != NULL );

	if(len < 0)
		len = strlen(string);

	while(len-- > 0)
		CallHookPkt(hook,(APTR)((ULONG)(*string++)),NULL);
}

/****************************************************************************/

STATIC VOID
format_date(const char *format,const struct tm *tm,struct Hook * hook)
{
	char buffer[40];
	const char * str;
	char c;

	assert( format != NULL && tm != NULL && hook != NULL);

	while((c = (*format++)) != '\0')
	{
		/* This is the simple case. */
		if(c != '%')
		{
			store_string_via_hook(&c,1,hook);
			continue;
		}

		/* OK, what kind of format is expected? */
		c = (*format++);

		/* We stop if the string ends here. Hm... should this count as an error? */
		if(c == '\0')
			break;

		switch(c)
		{
			/* Abbreviated weekday name ("Sun"). */
			case 'a':

				assert( 0 <= tm->tm_wday && tm->tm_wday <= 6 );

				if(0 <= tm->tm_wday && tm->tm_wday <= 6)
					str = __abbreviated_week_day_names[tm->tm_wday];
				else
					str = "-";

				store_string_via_hook(str,-1,hook);
				break;

			/* Full weekday name ("Sunday"). */
			case 'A':

				assert( 0 <= tm->tm_wday && tm->tm_wday <= 6 );

				if(0 <= tm->tm_wday && tm->tm_wday <= 6)
					str = __week_day_names[tm->tm_wday];
				else
					str = "-";

				store_string_via_hook(str,-1,hook);
				break;

			/* Abbreviated month name ("Jan"). */
			case 'b':

				assert( 0 <= tm->tm_mon && tm->tm_mon <= 11 );

				if(0 <= tm->tm_mon && tm->tm_mon <= 11)
					str = __abbreviated_month_names[tm->tm_mon];
				else
					str = "-";

				store_string_via_hook(str,-1,hook);
				break;

			/* Full month name ("January"). */
			case 'B':

				assert( 0 <= tm->tm_mon && tm->tm_mon <= 11 );

				if(0 <= tm->tm_mon && tm->tm_mon <= 11)
					str = __month_names[tm->tm_mon];
				else
					str = "";

				store_string_via_hook(str,-1,hook);
				break;

			/* Locale specific date and time ("%a %b %d %H:%M:%S %Y"). */
			case 'c':

				format_date("%a %b %d %H:%M:%S %Y",tm,hook);
				break;

			/* Day of the month ("01"-"31"). */
			case 'd':

				assert( 1 <= tm->tm_mday && tm->tm_mday <= 31 );

				__number_to_string((unsigned int)tm->tm_mday,buffer,sizeof(buffer),2);
				store_string_via_hook(buffer,2,hook);
				break;

			/* Hour ("00"-"23"). */
			case 'H':

				assert( 0 <= tm->tm_hour && tm->tm_hour <= 23 );

				__number_to_string((unsigned int)tm->tm_hour,buffer,sizeof(buffer),2);
				store_string_via_hook(buffer,2,hook);
				break;

			/* Hour ("00"-"12"). */
			case 'I':

				assert( 0 <= tm->tm_hour && tm->tm_hour <= 23 );

				__number_to_string((unsigned int)((tm->tm_hour > 12) ? (tm->tm_hour - 12) : tm->tm_hour),buffer,sizeof(buffer),2);
				store_string_via_hook(buffer,2,hook);
				break;

			/* Day of the year ("001"-"366"). */
			case 'j':

				__number_to_string(1 + __calculate_days_per_date(tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday) - __calculate_days_per_date(tm->tm_year+1900,1,1),buffer,sizeof(buffer),3);
				store_string_via_hook(buffer,-1,hook);
				break;

			/* Month ("01"-"12"). */
			case 'm':

				assert( 0 <= tm->tm_mon && tm->tm_mon <= 11 );

				__number_to_string((unsigned int)tm->tm_mon+1,buffer,sizeof(buffer),2);
				store_string_via_hook(buffer,2,hook);
				break;

			/* Minute ("00"-"59"). */
			case 'M':

				assert( 0 <= tm->tm_min && tm->tm_min <= 59 );

				__number_to_string((unsigned int)tm->tm_min,buffer,sizeof(buffer),2);
				store_string_via_hook(buffer,2,hook);
				break;

			/* 'Ante meridiem'/'Post meridiem' indicator. */
			case 'p':

				assert( 0 <= tm->tm_hour && tm->tm_hour <= 23 );

				store_string_via_hook((tm->tm_hour < 12) ? "AM" :" PM",2,hook);
				break;

			/* Seconds ("00"-"59"). */
			case 'S':

				assert( 0 <= tm->tm_sec && tm->tm_sec <= 59 );

				__number_to_string((unsigned int)tm->tm_sec,buffer,sizeof(buffer),2);
				store_string_via_hook(buffer,2,hook);
				break;

			/* Week number of the year; first week is the one that contains
			 * the first Sunday of the year ("00"-"53").
			 */
			case 'U':

				__number_to_string((tm->tm_yday + 7 - tm->tm_wday) / 7,buffer,sizeof(buffer),2);
				store_string_via_hook(buffer,2,hook);
				break;

			/* Week day ("0"-"6"). */
			case 'w':

				assert( 0 <= tm->tm_wday && tm->tm_wday <= 6 );

				__number_to_string((unsigned int)tm->tm_wday,buffer,sizeof(buffer),0);
				store_string_via_hook(buffer,1,hook);
				break;

			/* Week number of the year; first week is the one that contains
			 * the first Monday of the year ("00"-"53").
			 */
			case 'W':

				__number_to_string((tm->tm_yday + 7 - ((tm->tm_wday + 6) % 7)) / 7,buffer,sizeof(buffer),2);
				store_string_via_hook(buffer,2,hook);
				break;

			/* Locale-specific date ("%a %b %d, %Y"). */
			case 'x':

				format_date("%a %b %d, %Y",tm,hook);
				break;

			/* Locale-specific time ("%H:%M:%S"). */
			case 'X':

				format_date("%H:%M:%S",tm,hook);
				break;

			/* Year without century ("00"-"99"). */
			case 'y':

				assert( 0 <= tm->tm_year );

				__number_to_string((unsigned int)tm->tm_year % 100,buffer,sizeof(buffer),2);
				store_string_via_hook(buffer,2,hook);
				break;

			/* Year with century ("1970"-"2147483647"). */
			case 'Y':

				assert( 0 <= tm->tm_year );

				__number_to_string((unsigned int)1900 + tm->tm_year,buffer,sizeof(buffer),0);
				store_string_via_hook(buffer,-1,hook);
				break;

			/* Time zone name. */
			case 'Z':

				store_string_via_hook("GMT",3,hook);

				__locale_lock();

				if(__default_locale != NULL)
				{
					int hours_west_of_gmt;
					char sign;

					hours_west_of_gmt = __default_locale->loc_GMTOffset / 60;
					if(hours_west_of_gmt < 0)
					{
						sign = '+';

						hours_west_of_gmt = (-hours_west_of_gmt);
					}
					else if (hours_west_of_gmt > 0)
					{
						sign = '-';
					}

					if(hours_west_of_gmt != 0)
					{
						store_string_via_hook(&sign,1,hook);
						__number_to_string((unsigned int)hours_west_of_gmt,buffer,sizeof(buffer),2);
						store_string_via_hook(buffer,-1,hook);
					}
				}

				__locale_unlock();

				break;

			/* Store that character 'as is'. */
			default:

				store_string_via_hook(&c,1,hook);
				break;
		}
	}
}

/****************************************************************************/

size_t
strftime(char *s, size_t maxsize, const char *format, const struct tm *tm)
{
	DECLARE_LOCALEBASE();
	size_t result = 0;

	ENTER();

	SHOWPOINTER(s);
	SHOWVALUE(maxsize);
	SHOWSTRING(format);
	SHOWPOINTER(tm);

	assert( s != NULL && format != NULL && tm != NULL );
	assert( (int)maxsize >= 0 );

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(s == NULL || format == NULL || tm == NULL)
		{
			SHOWMSG("invalid parameters");

			__set_errno(EFAULT);
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	if(maxsize > 0)
	{
		struct format_hook_data data;
		struct Hook hook;

		data.len		= 0;
		data.buffer		= s;
		data.max_size	= maxsize-1;

		memset(&hook,0,sizeof(hook));

		#if defined(__amigaos4__)
		{
			hook.h_Entry	= (HOOKFUNC)format_hook_function;
		}
		#else
		{
			hook.h_Entry	= (HOOKFUNC)HookEntry;
			hook.h_SubEntry	= (HOOKFUNC)format_hook_function;
		}
		#endif /* __amigaos4__ */

		hook.h_Data = &data;

		__locale_lock();

		/* Try to use the locale.library date/time conversion function. */
		if(__locale_table[LC_TIME] != NULL)
		{
			struct DateStamp ds;
			struct tm tm_copy;
			time_t time_value;

			tm_copy = (*tm);

			time_value = mktime(&tm_copy);
			if(time_value == (time_t)-1)
			{
				SHOWMSG("invalid time");
				goto out;
			}

			/* Convert the number of seconds into a DateStamp, as to be
			   submitted to the FormatDate() function below. */
			if(CANNOT __convert_time_to_datestamp(time_value,&ds))
			{
				SHOWMSG("time conversion to datestamp failed");
				goto out;
			}

			assert( LocaleBase != NULL );

			FormatDate(__locale_table[LC_TIME],(STRPTR)format,&ds,&hook);
		}
		else
		{
			struct tm copy_tm;

			/* Fill in the week day if it's not in proper range. */
			if(tm->tm_wday < 0 || tm->tm_wday > 6)
			{
				/* We use a peculiar algorithm rather than falling back onto
				   mktime() here in order to avoid trouble with skewed results
				   owing to time zone influence. */
				copy_tm = (*tm);
				copy_tm.tm_wday = __calculate_weekday(tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);

				tm = &copy_tm;
			}

			format_date(format,tm,&hook);
		}

		__locale_unlock();

		(*data.buffer) = '\0';

		SHOWSTRING(s);

		result = data.len;
	}

 out:

	RETURN(result);
	return(result);
}
