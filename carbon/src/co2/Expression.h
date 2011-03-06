/*
 * carbon Compiler and programming language for libco2
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef Expression_H
#define Expression_H

#include "co2/CompileObject.h"

O_METHOD_DEF (Expression, void, set_scope, (void *_self, void *_scope));
O_METHOD_DEF (Expression, void, generate_left, (void *_self, bool left));

#define ExpressionClass_Attr			\
  CompileObjectClass_Attr;			\
  O_METHOD(Expression, set_scope);		\
  O_METHOD(Expression, generate_left)

#define Expression_Attr				\
  CompileObject_Attr;				\
  struct Type * type

O_CLASS (Expression, CompileObject);

void Expression_generate_actual_argument (void *_arg, va_list * app);

#endif /* Expression_H */
