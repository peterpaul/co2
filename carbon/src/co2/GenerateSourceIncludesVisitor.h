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
#ifndef GenerateSourceIncludesVisitor_H
#define GenerateSourceIncludesVisitor_H

#include "co2/BaseCompileObjectVisitor.h"

#define GenerateSourceIncludesVisitorClass_Attr		\
  BaseCompileObjectVisitorClass_Attr

#define GenerateSourceIncludesVisitor_Attr	\
  BaseCompileObjectVisitor_Attr;		\
  FILE *fp;					\
  struct Hash *map

O_CLASS(GenerateSourceIncludesVisitor, BaseCompileObjectVisitor);

#endif /* GenerateSourceIncludesVisitor_H */
