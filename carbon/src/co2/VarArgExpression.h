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
#ifndef VarArgExpression_H
#define VarArgExpression_H

#include "co2/Expression.h"
#include "co2/Type.h"

#define VarArgExpressionClass_Attr			\
  ExpressionClass_Attr

#define VarArgExpression_Attr				\
  Expression_Attr;					\
  struct Expression * va_arg_expr;			\
  struct Type * va_arg_type

O_CLASS (VarArgExpression, Expression);

#endif /* VarArgExpression_H */
