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

#define __C_MACROS__
#include <ctype.h>

/****************************************************************************/

/* The construction of this table is rather peculiar. The assumption is that
   the table index from which a flag value could be fetched is in the range
   -128..255 since the character to be tested could be a signed or unsigned
   8 bit value. The table access pointer (declared below) therefore points
   to the 128th byte of '__ctype_data[]'. */
static const unsigned char __ctype_data[3 * 128] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	/*   0      */ __CTYPE_CONTROL,
	/*   1      */ __CTYPE_CONTROL,
	/*   2      */ __CTYPE_CONTROL,
	/*   3      */ __CTYPE_CONTROL,
	/*   4      */ __CTYPE_CONTROL,
	/*   5      */ __CTYPE_CONTROL,
	/*   6      */ __CTYPE_CONTROL,
	/*   7      */ __CTYPE_CONTROL,
	/*   8      */ __CTYPE_CONTROL,
	/*   9      */ __CTYPE_CONTROL|__CTYPE_WHITE_SPACE,
	/*  10      */ __CTYPE_CONTROL|__CTYPE_WHITE_SPACE,
	/*  11      */ __CTYPE_CONTROL|__CTYPE_WHITE_SPACE,
	/*  12      */ __CTYPE_CONTROL|__CTYPE_WHITE_SPACE,
	/*  13      */ __CTYPE_CONTROL|__CTYPE_WHITE_SPACE,
	/*  14      */ __CTYPE_CONTROL,
	/*  15      */ __CTYPE_CONTROL,
	/*  16      */ __CTYPE_CONTROL,
	/*  17      */ __CTYPE_CONTROL,
	/*  18      */ __CTYPE_CONTROL,
	/*  19      */ __CTYPE_CONTROL,
	/*  20      */ __CTYPE_CONTROL,
	/*  21      */ __CTYPE_CONTROL,
	/*  22      */ __CTYPE_CONTROL,
	/*  23      */ __CTYPE_CONTROL,
	/*  24      */ __CTYPE_CONTROL,
	/*  25      */ __CTYPE_CONTROL,
	/*  26      */ __CTYPE_CONTROL,
	/*  27      */ __CTYPE_CONTROL,
	/*  28      */ __CTYPE_CONTROL,
	/*  29      */ __CTYPE_CONTROL,
	/*  30      */ __CTYPE_CONTROL,
	/*  31      */ __CTYPE_CONTROL,
	/*  32, ' ' */ __CTYPE_PRINTABLE|__CTYPE_WHITE_SPACE,
	/*  33, '!' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  34, '"' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  35, '#' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  36, '$' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  37, '%' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  38, '&' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  39, ''' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  40, '(' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  41, ')' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  42, '*' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  43, '+' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  44, ',' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  45, '-' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  46, '.' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  47, '/' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  48, '0' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  49, '1' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  50, '2' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  51, '3' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  52, '4' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  53, '5' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  54, '6' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  55, '7' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  56, '8' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  57, '9' */ __CTYPE_DIGIT|__CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE,
	/*  58, ':' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  59, ';' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  60, '<' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  61, '=' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  62, '>' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  63, '?' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  64, '@' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  65, 'A' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  66, 'B' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  67, 'C' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  68, 'D' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  69, 'E' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  70, 'F' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  71, 'G' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  72, 'H' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  73, 'I' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  74, 'J' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  75, 'K' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  76, 'L' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  77, 'M' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  78, 'N' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  79, 'O' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  80, 'P' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  81, 'Q' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  82, 'R' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  83, 'S' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  84, 'T' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  85, 'U' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  86, 'V' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  87, 'W' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  88, 'X' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  89, 'Y' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  90, 'Z' */ __CTYPE_PRINTABLE|__CTYPE_UPPER_CASE,
	/*  91, '[' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  92, '\' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  93, ']' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  94, '^' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  95, '_' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  96, '`' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/*  97, 'a' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/*  98, 'b' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/*  99, 'c' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 100, 'd' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 101, 'e' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 102, 'f' */ __CTYPE_HEX_DIGIT|__CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 103, 'g' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 104, 'h' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 105, 'i' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 106, 'j' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 107, 'k' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 108, 'l' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 109, 'm' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 110, 'n' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 111, 'o' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 112, 'p' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 113, 'q' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 114, 'r' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 115, 's' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 116, 't' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 117, 'u' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 118, 'v' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 119, 'w' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 120, 'x' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 121, 'y' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 122, 'z' */ __CTYPE_PRINTABLE|__CTYPE_LOWER_CASE,
	/* 123, '{' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/* 124, '|' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/* 125, '}' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/* 126, '~' */ __CTYPE_PUNCTUATION|__CTYPE_PRINTABLE,
	/* 127      */ __CTYPE_CONTROL,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/****************************************************************************/

const unsigned char * const __ctype_table = &__ctype_data[128];
