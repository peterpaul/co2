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
#ifndef FunctionType_H
#define FunctionType_H

#include "co2/Type.h"
#include "co2/RefList.h"

O_METHOD_DEF (FunctionType, void *, ctor_from_decl,
	      (void *_self, va_list * app));
O_METHOD_DEF (FunctionType, void *, ctor_from_expr,
	      (void *_self, va_list * app));
O_METHOD_DEF (FunctionType, void, generate_named, 
	      (void *_self, struct Token *name));
#define FunctionTypeClass_Attr			\
  TypeClass_Attr;				\
  O_METHOD(FunctionType, ctor_from_decl);	\
  O_METHOD(FunctionType, ctor_from_expr);	\
  O_METHOD(FunctionType, generate_named)

#define FunctionType_Attr			\
  Type_Attr;					\
  bool has_var_args;				\
  struct Type * return_type;			\
  struct RefList * parameters;			\
  struct Token * generated_name

O_CLASS (FunctionType, Type);

#endif /* FunctionType_H */
