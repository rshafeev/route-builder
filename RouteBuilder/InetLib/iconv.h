/* Copyright (C) 1999-2003 Free Software Foundation, Inc.
	   This file is part of the GNU LIBICONV Library.
	
	   The GNU LIBICONV Library is free software; you can redistribute it
	   and/or modify it under the terms of the GNU Library General Public
6	   License as published by the Free Software Foundation; either version 2
7	   of the License, or (at your option) any later version.
8	
9	   The GNU LIBICONV Library is distributed in the hope that it will be
10	   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
11	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
12	   Library General Public License for more details.
13	
14	   You should have received a copy of the GNU Library General Public
15	   License along with the GNU LIBICONV Library; see the file COPYING.LIB.
16	   If not, write to the Free Software Foundation, Inc., 59 Temple Place -
17	   Suite 330, Boston, MA 02111-1307, USA.  */
	
	/* When installed, this file is called "iconv.h". */
	
	#ifndef _LIBICONV_H
	#define _LIBICONV_H
	
	#define _LIBICONV_VERSION 0x0109    /* version number: (major<<8) + minor */
	extern int _libiconv_version;       /* Likewise */
	
	/* We would like to #include any system header file which could define
28	   iconv_t, 1. in order to eliminate the risk that the user gets compilation
29	   errors because some other system header file includes /usr/include/iconv.h
30	   which defines iconv_t or declares iconv after this file, 2. when compiling
31	   for LIBICONV_PLUG, we need the proper iconv_t type in order to produce
32	   binary compatible code.
33	   But gcc's #include_next is not portable. Thus, once libiconv's iconv.h
34	   has been installed in /usr/local/include, there is no way any more to
35	   include the original /usr/include/iconv.h. We simply have to get away
36	   without it.
37	   Ad 1. The risk that a system header file does
38	   #include "iconv.h"  or  #include_next "iconv.h"
39	   is small. They all do #include <iconv.h>.
40	   Ad 2. The iconv_t type is a pointer type in all cases I have seen. (It
41	   has to be a scalar type because (iconv_t)(-1) is a possible return value
42	   from iconv_open().) */
	
	/* Define iconv_t ourselves. */
	#undef iconv_t
	#define iconv_t libiconv_t
	typedef void* iconv_t;
	
	/* Get size_t declaration. */
	#include <stddef.h>
	
	/* Get errno declaration and values. */
	#include <errno.h>
	/* Some systems, like SunOS 4, don't have EILSEQ. Some systems, like BSD/OS,
55	   have EILSEQ in a different header.  On these systems, define EILSEQ
56	   ourselves. */
	#ifndef EILSEQ
	#define EILSEQ @EILSEQ@
	#endif
	
	
	#ifdef __cplusplus
	extern "C" {
	#endif
	
	
	/* Allocates descriptor for code conversion from encoding `fromcode' to
68	   encoding `tocode'. */
	#ifndef LIBICONV_PLUG
	#define iconv_open libiconv_open
	#endif
	extern iconv_t iconv_open (const char* tocode, const char* fromcode);
	
	/* Converts, using conversion descriptor `cd', at most `*inbytesleft' bytes
75	   starting at `*inbuf', writing at most `*outbytesleft' bytes starting at
76	   `*outbuf'.
77	   Decrements `*inbytesleft' and increments `*inbuf' by the same amount.
78	   Decrements `*outbytesleft' and increments `*outbuf' by the same amount. */
	#ifndef LIBICONV_PLUG
	#define iconv libiconv
	#endif
	extern size_t iconv (iconv_t cd, const char* * inbuf, size_t *inbytesleft, char* * outbuf, size_t *outbytesleft);
	
	/* Frees resources allocated for conversion descriptor `cd'. */
	#ifndef LIBICONV_PLUG
	#define iconv_close libiconv_close
	#endif
	extern int iconv_close (iconv_t cd);
	
	
	#ifndef LIBICONV_PLUG
	
	/* Nonstandard extensions. */
	
	/* Control of attributes. */
	#define iconvctl libiconvctl
	extern int iconvctl (iconv_t cd, int request, void* argument);
	
	/* Requests for iconvctl. */
	#define ICONV_TRIVIALP            0  /* int *argument */
	#define ICONV_GET_TRANSLITERATE   1  /* int *argument */
	#define ICONV_SET_TRANSLITERATE   2  /* const int *argument */
	#define ICONV_GET_DISCARD_ILSEQ   3  /* int *argument */
	#define ICONV_SET_DISCARD_ILSEQ   4  /* const int *argument */
	
	/* Listing of locale independent encodings. */
	#define iconvlist libiconvlist
	extern void iconvlist (int (*do_one) (unsigned int namescount,
	                                      const char * const * names,
	                                      void* data),
	                       void* data);
	
	/* Support for relocatable packages.  */
	
	/* Sets the original and the current installation prefix of the package.
116	   Relocation simply replaces a pathname starting with the original prefix
117	   by the corresponding pathname with the current prefix instead.  Both
118	   prefixes should be directory names without trailing slash (i.e. use ""
119	   instead of "/").  */
	extern void libiconv_set_relocation_prefix (const char *orig_prefix,
	                                            const char *curr_prefix);
	
	#endif
	#ifdef __cplusplus
	}
	#endif
	
	
	#endif /* _LIBICONV_H */