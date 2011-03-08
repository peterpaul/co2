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
#include "FinallyStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT(FinallyStatement, void *, ctor, (void *_self, va_list *app))
{
  struct FinallyStatement * self = O_CAST(_self, FinallyStatement());
  self = O_SUPER->ctor(self, app);
  self->body = O_RETAIN_ARG (Statement);
  return self;
}

O_IMPLEMENT(FinallyStatement, void *, dtor, (void *_self))
{
  struct FinallyStatement *self = O_CAST(_self, FinallyStatement());
  O_CALL (self->body, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (FinallyStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct FinallyStatement *self = O_CAST (_self, FinallyStatement ());
  O_CALL (self->body, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(FinallyStatement, void, type_check, (void *_self))
{
  struct FinallyStatement *self = O_CAST(_self, FinallyStatement());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->body, type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT(FinallyStatement, void, generate, (void *_self))
{
  struct FinallyStatement *self = O_CAST(_self, FinallyStatement());
  O_CALL (self->body, generate);
}

O_OBJECT(FinallyStatement, Statement);
O_OBJECT_METHOD(FinallyStatement, ctor);
O_OBJECT_METHOD(FinallyStatement, dtor);
O_OBJECT_METHOD(FinallyStatement, accept);
O_OBJECT_METHOD(FinallyStatement, type_check);
O_OBJECT_METHOD(FinallyStatement, generate);
O_END_OBJECT
