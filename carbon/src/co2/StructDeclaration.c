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
#include "co2/StructDeclaration.h"
#include "co2/TypeDeclaration.h"
#include "co2/ObjectType.h"
#include "co2/IScope.h"
#include "co2/io.h"

#define O_SUPER ObjectTypeDeclaration()

O_IMPLEMENT(StructDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct StructDeclaration * self = O_CAST(_self, StructDeclaration());
  self = O_SUPER->ctor(self, app);
  self->member_scope = O_BRANCH_GET_ARG_IF (IScope);
  self->members = O_BRANCH_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT(StructDeclaration, void *, dtor, (void *_self))
{
  struct StructDeclaration *self = O_CAST(_self, StructDeclaration());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (StructDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct StructDeclaration *self = O_CAST (_self, StructDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->members, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (StructDeclaration, void, type_check, (void *_self))
{
  struct StructDeclaration *self = O_CAST (_self, StructDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->members, map, CompileObject_type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (StructDeclaration, bool, is_compatible,
	     (void *_self, void *_other))
{
  struct StructDeclaration *self = O_CAST (_self, StructDeclaration ());
  struct StructDeclaration *other;
  if (o_is_of (_other, TypeDeclaration ()))
    {
      struct TypeDeclaration * type_decl = O_CAST (_other, TypeDeclaration ());
      struct ObjectType * object_type = O_CAST (type_decl->type, ObjectType ());
      other = o_cast (object_type->decl, StructDeclaration ());
    }
  else
    {
      other = o_cast (_other, StructDeclaration ());
    }
      return self == other;
}

O_OBJECT(StructDeclaration, ObjectTypeDeclaration);
O_OBJECT_METHOD(StructDeclaration, ctor);
O_OBJECT_METHOD(StructDeclaration, dtor);
O_OBJECT_METHOD(StructDeclaration, accept);
O_OBJECT_METHOD(StructDeclaration, type_check);
O_OBJECT_METHOD(StructDeclaration, is_compatible);
O_END_OBJECT
