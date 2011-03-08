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
#ifndef NewExpression_H
#define NewExpression_H

#include "co2/TokenExpression.h"

O_METHOD_DEF (NewExpression, void, set_ctor_name,
	      (void *_self, struct TokenExpression * ctor_name));

#define NewExpressionClass_Attr			\
  ExpressionClass_Attr;				\
  O_METHOD(NewExpression, set_ctor_name)

#define NewExpression_Attr			\
  Expression_Attr;				\
  struct Type * new_type;			\
  struct TokenExpression * ctor_name;		\
  struct Expression * array_size;		\
  struct RefList * ctor_arguments

O_CLASS (NewExpression, Expression);

#endif /* NewExpression_H */
