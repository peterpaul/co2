/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011,2012 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef UTILS_H
#define UTILS_H

#include <assert.h>

#ifndef __ASSERT_VOID_CAST
#define __ASSERT_VOID_CAST (void)
#endif

/* BSD/glibc's <sys/cdefs.h> provides this; MinGW's headers don't, so
 * __STRING(x) is left unexpanded and the compiler tries to call a
 * nonexistent function named __STRING with x as a bare argument -- which
 * fails to compile whenever x isn't itself a declared identifier (e.g. the
 * method-name tokens `ctor`/`dtor` passed in by O_CALL below, which are only
 * ever valid as struct member names, never as standalone variables). */
#ifndef __STRING
#define __STRING(x) #x
#endif

#ifdef __GLIBC__
#define ASSERT_FAIL(p,file,line,function)     \
  __assert_fail(p,file,line,function)
#else
#define ASSERT_FAIL(p,file,line,function)     \
  __assert_rtn(function,file,line,p)
#endif

#define message(x,...)                                                  \
  __write_message(x, __FILE__, __LINE__,				\
                  __PRETTY_FUNCTION__, ## __VA_ARGS__)
#define assertFalse(p,msg,...)						\
  ((p)                                                                  \
   ? (message(msg, ## __VA_ARGS__),                                     \
      ASSERT_FAIL("!" __STRING(p), __FILE__,                          \
                    __LINE__, __PRETTY_FUNCTION__))                     \
   : __ASSERT_VOID_CAST(0))
#define assertTrue(p,msg,...)                                           \
  ((p)                                                                  \
   ? __ASSERT_VOID_CAST(0)                                              \
   : (message(msg, ## __VA_ARGS__),					\
      ASSERT_FAIL(__STRING(p), __FILE__,                              \
                    __LINE__, __PRETTY_FUNCTION__)))

void __write_message(const char *fmt,
		     const char *file,
		     unsigned int line, const char *function, ...)
    __attribute__ ((format(printf, 1, 5)));

unsigned long hash_function(const unsigned char *str);
int ipow(int base, int exp);
unsigned upow(unsigned base, int exp);

#if !defined(false) && !defined(__cplusplus) && \
    (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L)
typedef int bool;
#define false 0
#define true !false
#endif

#define MAX_FUN(type) type max_##type(type a, type b){return (a > b ? a : b);}
#define MIN_FUN(type) type min_##type(type a, type b){return (a < b ? a : b);}

#endif				/* UTILS_H */
