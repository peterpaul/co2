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
#include "co2/WhileStatement.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT (WhileStatement, void *, ctor, (void *_self, va_list * app))
{
  struct WhileStatement *self = O_CAST (_self, WhileStatement ());
  self = O_SUPER->ctor (self, app);
  self->condition = O_RETAIN_ARG (Expression);
  self->body = O_RETAIN_ARG (Statement);
  return self;
}

O_IMPLEMENT (WhileStatement, void *, dtor, (void *_self))
{
  struct WhileStatement *self = O_CAST (_self, WhileStatement ());
  O_CALL (self->condition, release);
  O_CALL (self->body, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (WhileStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct WhileStatement *self = O_CAST (_self, WhileStatement ());
  O_CALL (self->condition, accept, visitor);
  O_CALL (self->body, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (WhileStatement, void, type_check, (void *_self))
{
  struct WhileStatement *self = O_CAST (_self, WhileStatement ());
  O_CALL (self->condition, type_check);
  O_CALL (self->body, type_check);
}

O_IMPLEMENT (WhileStatement, void, generate, (void *_self))
{
  struct WhileStatement *self = O_CAST (_self, WhileStatement ());
  fprintf (out, "while (");
  O_CALL (self->condition, generate);
  fprintf (out, ")\n");
  O_CALL (self->body, generate);
}

O_OBJECT (WhileStatement, Statement);
O_OBJECT_METHOD (WhileStatement, ctor);
O_OBJECT_METHOD (WhileStatement, dtor);
O_OBJECT_METHOD (WhileStatement, accept);
O_OBJECT_METHOD (WhileStatement, type_check);
O_OBJECT_METHOD (WhileStatement, generate);
O_END_OBJECT
