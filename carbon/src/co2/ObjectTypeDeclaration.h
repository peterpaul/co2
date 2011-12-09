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
#ifndef ObjectTypeDeclaration_H
#define ObjectTypeDeclaration_H

#include "co2/Declaration.h"

O_METHOD_DEF (ObjectTypeDeclaration, bool, is_compatible,
	      (void *_self, void *_other));

#define ObjectTypeDeclarationClass_Attr			\
  DeclarationClass_Attr;				\
  O_METHOD(ObjectTypeDeclaration, is_compatible)

#define ObjectTypeDeclaration_Attr		\
  Declaration_Attr;				\
  struct RefList * members;			\
  struct IScope * member_scope

O_CLASS (ObjectTypeDeclaration, Declaration);

void ObjectTypeDeclaration_generate_method_arguments (void *_arg);
void ObjectTypeDeclaration_generate_method_argument_names (void *_arg);
void ObjectTypeDeclaration_generate_method_registration_2 (void *_method_decl, va_list * app);

#endif /* ObjectTypeDeclaration_H */
