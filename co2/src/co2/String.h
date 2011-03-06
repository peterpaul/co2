/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef STRING_H
#define STRING_H

#include "co2/RefObject.h"

O_METHOD_DEF(String, void *, resize, (void *_self, int size));
O_METHOD_DEF(String, void *, ensure, (void *_self, int size));
O_METHOD_DEF(String, void *, append, (void *_self, struct String * str));
O_METHOD_DEF(String, void *, append_str, (void *_self, char *str, ...));
O_METHOD_DEF(String, void *, append_str_n,
	     (void *_self, char *str, int n));
O_METHOD_DEF(String, void *, replace,
	     (void *_self, struct String * that, struct String * this));
O_METHOD_DEF(String, void *, ctor_from_file, (void *_self, va_list * app));
/**
 * @brief Prints this String to a file.
 *
 * Upon  successful  return,  returns the number of characters printed (not
 * including the '\\0' used to end output to strings).
 * @param _self The String to print
 * @param fp The FILE to write to
 * @return The number of characters written to the FILE
 */
O_METHOD_DEF(String, int, fprint, (void *_self, FILE * fp));
/**
 * @brief Prints this String to a string.
 *
 * Upon  successful  return,  returns the number of characters printed (not
 * including the '\\0' used to end output to strings). Does not write more than
 * size bytes (including the '\\0').  If the output was truncated due to this
 * limit then the return value is the number of characters (not including  the
 * '\\0') which would have been written to the final string if enough space had
 * been available. Thus, a return value of size or more means that the output
 * was truncated.
 * @param _self The String to print
 * @param str The string to write to
 * @param size The size of the target string
 * @return The number of characters written to the string
 */
O_METHOD_DEF(String, int, snprint, (void *_self, char *str, int size));
O_METHOD_DEF(String, struct List *, split, (void *_self, const char * delim));

#define StringClass_Attr						\
	RefObjectClass_Attr;						\
	O_METHOD (String, resize);					\
	O_METHOD (String, ensure);					\
	O_METHOD (String, append);					\
	O_METHOD (String, append_str);					\
	O_METHOD (String, append_str_n);				\
	O_METHOD (String, replace);					\
	O_METHOD (String, fprint);					\
	O_METHOD (String, snprint);					\
	O_METHOD (String, ctor_from_file);				\
	O_METHOD (String, split);					\

#define String_Attr							\
	RefObject_Attr;							\
	char * data;							\
	int length, max

O_CLASS(String, RefObject);

#endif
