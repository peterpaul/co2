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
#ifndef TryStatement_H
#define TryStatement_H

#include "co2/Statement.h"

#define TryStatementClass_Attr			\
  StatementClass_Attr

#define TryStatement_Attr			\
  Statement_Attr;				\
  struct Statement * try_clause;		\
  struct RefList * catch_clause;		\
  struct Statement * finally_clause

O_CLASS(TryStatement, Statement);

#endif /* TryStatement_H */
