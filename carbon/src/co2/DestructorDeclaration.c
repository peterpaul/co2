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
#include "co2/DestructorDeclaration.h"
#include "co2/Type.h"
#include "co2/RefList.h"
#include "co2/Statement.h"
#include "co2/io.h"
#include "co2/FunctionType.h"

#define O_SUPER Declaration()

O_IMPLEMENT (DestructorDeclaration, void *, ctor,
	     (void *_self, va_list * app))
{
  struct DestructorDeclaration *self =
    O_CAST (_self, DestructorDeclaration ());
  self = O_SUPER->ctor (self, app);
  /*
     // TODO lookup current class
     self->type = O_CAST(va_arg(*app, struct FunctionType *), FunctionType());
     O_CALL(self->type, retain);
   */
  self->class_name = O_RETAIN_ARG (Token);
  self->body = O_BRANCH_RETAIN_ARG (Statement);
  return self;
}

O_IMPLEMENT (DestructorDeclaration, void *, dtor, (void *_self))
{
  struct DestructorDeclaration *self =
    O_CAST (_self, DestructorDeclaration ());
  O_CALL (self->class_name, release);
  O_BRANCH_CALL (self->body, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (DestructorDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct DestructorDeclaration *self = O_CAST (_self, DestructorDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->body, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (DestructorDeclaration, void, type_check, (void *_self))
{
  struct DestructorDeclaration *self =
    O_CAST (_self, DestructorDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->body, type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_OBJECT (DestructorDeclaration, Declaration);
O_OBJECT_METHOD (DestructorDeclaration, ctor);
O_OBJECT_METHOD (DestructorDeclaration, dtor);
O_OBJECT_METHOD (DestructorDeclaration, accept);
O_OBJECT_METHOD (DestructorDeclaration, type_check);
O_END_OBJECT
