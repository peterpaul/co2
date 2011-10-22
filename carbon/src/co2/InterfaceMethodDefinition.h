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
#ifndef InterfaceMethodDefinition_H
#define InterfaceMethodDefinition_H

#include "co2/CompileObject.h"
#include "co2/Token.h"

#define InterfaceMethodDefinitionClass_Attr	\
  CompileObjectClass_Attr

#define InterfaceMethodDefinition_Attr		\
  CompileObject_Attr;				\
  struct Token * interface_name;		\
  struct Token * method_name;			\
  struct InterfaceDeclaration * interface_decl;	\
  struct FunctionDeclaration * method_decl

O_CLASS(InterfaceMethodDefinition, CompileObject);

#endif /* InterfaceMethodDefinition_H */
