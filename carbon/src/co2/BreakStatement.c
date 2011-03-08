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
#include "co2/BreakStatement.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT(BreakStatement, void *, ctor, (void *_self, va_list *app))
{
  struct BreakStatement * self = O_CAST(_self, BreakStatement());
  self = O_SUPER->ctor(self, app);
  return self;
}

O_IMPLEMENT(BreakStatement, void *, dtor, (void *_self))
{
  struct BreakStatement *self = O_CAST(_self, BreakStatement());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(BreakStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct BreakStatement *self = O_CAST(_self, BreakStatement());
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(BreakStatement, void, type_check, (void *_self))
{
  struct BreakStatement *self = O_CAST(_self, BreakStatement());
}

O_IMPLEMENT(BreakStatement, void, generate, (void *_self))
{
  struct BreakStatement *self = O_CAST(_self, BreakStatement());
  fprintf (out, "break;\n");
}

O_OBJECT(BreakStatement, Statement);
O_OBJECT_METHOD(BreakStatement, ctor);
O_OBJECT_METHOD(BreakStatement, dtor);
O_OBJECT_METHOD(BreakStatement, accept);
O_OBJECT_METHOD(BreakStatement, type_check);
O_OBJECT_METHOD(BreakStatement, generate);
O_END_OBJECT
