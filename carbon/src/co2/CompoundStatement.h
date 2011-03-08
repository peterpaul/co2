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
#ifndef CompoundStatement_H
#define CompoundStatement_H

#include "co2/Statement.h"
#include "co2/RefList.h"

#define CompoundStatementClass_Attr			\
  StatementClass_Attr

#define CompoundStatement_Attr				\
  Statement_Attr;					\
  struct RefList * body

O_CLASS (CompoundStatement, Statement);

#endif /* CompoundStatement_H */
