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
#ifndef CompileObject_H
#define CompileObject_H

#include "co2/RefObject.h"
#include "co2/Context.h"
#include "co2/BaseCompileObjectVisitor.h"

O_METHOD_DEF (CompileObject, void, type_check, (void *_self));
O_METHOD_DEF (CompileObject, void, optimize, (void *_self));
O_METHOD_DEF (CompileObject, void, generate, (void *_self));
O_METHOD_DEF (CompileObject, struct Token *, get_token, (void *_self));
O_METHOD_DEF (CompileObject, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor));

#define CompileObjectClass_Attr			\
  RefObjectClass_Attr;				\
  O_METHOD(CompileObject, generate);		\
  O_METHOD(CompileObject, optimize);		\
  O_METHOD(CompileObject, type_check);		\
  O_METHOD(CompileObject, get_token);		\
  O_METHOD(CompileObject, accept)

#define CompileObject_Attr			\
  RefObject_Attr

O_CLASS (CompileObject, RefObject);

int type_filter (void *_member, va_list * app);
int not_type_filter (void *_member, va_list * app);
void accept (void *_object, va_list *app);
void CompileObject_type_check (void *_object);
void CompileObject_generate (void *_object);

#endif /* CompileObject_H */
