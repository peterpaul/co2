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
#include "co2/CatchStatement.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/ObjectType.h"

#define O_SUPER Statement()

O_IMPLEMENT(CatchStatement, void *, ctor, (void *_self, va_list *app))
{
  struct CatchStatement * self = O_CAST(_self, CatchStatement());
  self = O_SUPER->ctor(self, app);
  self->scope = O_GET_ARG_IF (IScope);
  self->argument = O_RETAIN_ARG (ArgumentDeclaration);
  self->body = O_RETAIN_ARG (Statement);
  return self;
}

O_IMPLEMENT(CatchStatement, void *, dtor, (void *_self))
{
  struct CatchStatement *self = O_CAST(_self, CatchStatement());
  O_CALL (self->scope, delete);
  O_CALL (self->argument, release);
  O_CALL (self->body, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (CatchStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct CatchStatement *self = O_CAST (_self, CatchStatement ());
  O_CALL (self->argument, accept, visitor);
  O_CALL (self->body, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(CatchStatement, void, type_check, (void *_self))
{
  struct CatchStatement *self = O_CAST(_self, CatchStatement());
  O_BRANCH_CALL (current_context, add, self);
  if (!o_is_of (self->argument->type, ObjectType ()))
    {
      error (self->argument->name, "Expected class.\n");
    }
  O_CALL (self->argument, type_check);
  O_CALL (self->body, type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT(CatchStatement, void, generate, (void *_self))
{
  struct CatchStatement *self = O_CAST(_self, CatchStatement());
  O_CALL (self->body, generate);
  
}

O_OBJECT(CatchStatement, Statement);
O_OBJECT_METHOD(CatchStatement, ctor);
O_OBJECT_METHOD(CatchStatement, dtor);
O_OBJECT_METHOD(CatchStatement, accept);
O_OBJECT_METHOD(CatchStatement, type_check);
O_OBJECT_METHOD(CatchStatement, generate);
O_END_OBJECT
