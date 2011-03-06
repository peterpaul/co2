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
#ifndef CONSTRUCTORDECLARATION_H
#define CONSTRUCTORDECLARATION_H

#include "co2/Declaration.h"

O_METHOD_DEF (ConstructorDeclaration, bool, has_var_args, (void *_self));

#define ConstructorDeclarationClass_Attr		\
  DeclarationClass_Attr;				\
  O_METHOD (ConstructorDeclaration, has_var_args)

#define ConstructorDeclaration_Attr		\
  Declaration_Attr;				\
  struct Token * class_name;			\
  struct RefList * formal_arguments;		\
  struct Statement * body

O_CLASS (ConstructorDeclaration, Declaration);

#endif /* CONSTRUCTORDECLARATION_H */
