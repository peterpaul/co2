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
#ifndef Declaration_H
#define Declaration_H

#include "co2/CompileObject.h"
#include "co2/Scope.h"

O_METHOD_DEF (Declaration, void, set_scope, (void *_self, void *scope));
O_METHOD_DEF (Declaration, void, set_class_decl,
	      (void *_self, void *_class_decl));
O_METHOD_DEF (Declaration, void, set_include_file,
	      (void *_self, void *_include_file));

#define DeclarationClass_Attr			\
  CompileObjectClass_Attr;			\
  O_METHOD(Declaration, set_scope);		\
  O_METHOD(Declaration, set_class_decl);	\
  O_METHOD(Declaration, set_include_file)

#define Declaration_Attr			\
  CompileObject_Attr;				\
  struct Token * name;				\
  struct Scope * scope;				\
  struct Type * type;				\
  struct Token * include_file;			\
  struct ClassDeclaration * class_decl;		\
  struct File * file;				\
  bool declared;				\
  bool defined

O_CLASS (Declaration, CompileObject);

void Declaration_list_set_include_header (void *_decl, va_list * app);
void Declaration_list_type_check (void *_decl);
int Declaration_new_member_filter (void *_member, va_list * app);

#endif /* Declaration_H */
