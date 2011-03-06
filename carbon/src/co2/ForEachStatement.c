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
#include "co2/ForEachStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT (ForEachStatement, void *, ctor, (void *_self, va_list * app))
{
  struct ForEachStatement *self = O_CAST (_self, ForEachStatement ());
  self = O_SUPER->ctor (self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT (ForEachStatement, void *, dtor, (void *_self))
{
  struct ForEachStatement *self = O_CAST (_self, ForEachStatement ());
  /* TODO cleanup */
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ForEachStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ForEachStatement *self = O_CAST (_self, ForEachStatement ());
  /* TODO implement */
  O_CALL (visitor, visit, self);
}

O_OBJECT (ForEachStatement, Statement);
O_OBJECT_METHOD (ForEachStatement, ctor);
O_OBJECT_METHOD (ForEachStatement, dtor);
O_OBJECT_METHOD (ForEachStatement, accept);
O_END_OBJECT
