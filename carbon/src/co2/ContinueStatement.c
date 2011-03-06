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
#include "co2/ContinueStatement.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT(ContinueStatement, void *, ctor, (void *_self, va_list *app))
{
  struct ContinueStatement * self = O_CAST(_self, ContinueStatement());
  self = O_SUPER->ctor(self, app);
  return self;
}

O_IMPLEMENT(ContinueStatement, void *, dtor, (void *_self))
{
  struct ContinueStatement *self = O_CAST(_self, ContinueStatement());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ContinueStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ContinueStatement *self = O_CAST(_self, ContinueStatement());
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(ContinueStatement, void, type_check, (void *_self))
{
  struct ContinueStatement *self = O_CAST(_self, ContinueStatement());
}

O_IMPLEMENT(ContinueStatement, void, generate, (void *_self))
{
  struct ContinueStatement *self = O_CAST(_self, ContinueStatement());
  fprintf (out, "break;\n");
}

O_OBJECT(ContinueStatement, Statement);
O_OBJECT_METHOD(ContinueStatement, ctor);
O_OBJECT_METHOD(ContinueStatement, dtor);
O_OBJECT_METHOD(ContinueStatement, accept);
O_OBJECT_METHOD(ContinueStatement, type_check);
O_OBJECT_METHOD(ContinueStatement, generate);
O_END_OBJECT
