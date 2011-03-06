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
#include "co2/ClassDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/VariableDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/Type.h"
#include "co2/ObjectType.h"
#include "co2/Token.h"
#include "co2/RefList.h"
#include "co2/FunctionType.h"
#include "co2/io.h"
#include "co2/Statement.h"
#include "co2/Scope.h"
#include "co2/PrimitiveType.h"
#include "grammar.h"

#define O_SUPER ObjectTypeDeclaration()

O_IMPLEMENT (ClassDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->superclass = O_BRANCH_RETAIN_ARG (Token);
  self->interfaces = O_BRANCH_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT (ClassDeclaration, void *, dtor, (void *_self))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  O_BRANCH_CALL (self->superclass, release);
  O_BRANCH_CALL (self->interfaces, release);
  return O_SUPER->dtor (self);  
}

O_IMPLEMENT (ClassDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->members, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

int
new_constructor_filter (void *_constructor)
{
  struct ConstructorDeclaration *constructor =
    O_CAST (_constructor, ConstructorDeclaration ());
  if (strcmp (constructor->name->name->data, "ctor") != 0)
    {
      return O_BRANCH_CALL (constructor->scope->parent,
			    find_type, constructor->name,
			    ConstructorDeclaration ()) == NULL;
    }
  return false;
}

void
generate_superclass (struct ClassDeclaration *self)
{
  if (self->superclass)
    {
      O_CALL (self->superclass, generate);
    }
  else
    {
      fprintf (out, "Object");
    }
}

O_IMPLEMENT (ClassDeclaration, void, type_check, (void *_self))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  if (self->superclass)
    {
      struct Declaration *_super_class =
	O_CALL (self->scope, lookup, self->superclass);
      if (_super_class)
	{
	  struct ClassDeclaration *super_class =
	    O_CAST (_super_class, ClassDeclaration ());
	  self->member_scope->parent = super_class->member_scope;
	}
    }
  /* TODO check whether:
   * 1. superclasses already implement interfaces, if so: fail
   * 2. foreach interface, methods already exists in other interface (including interfaces from superclasses), if so: fail
   * 3. class (including superclasses) implements all methods of all interfaces, if not: fail
   */
  O_CALL (self->members, map, CompileObject_type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (ClassDeclaration, bool, is_compatible,
	     (void *_self, void *_other))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  if (o_is_of (_other, ClassDeclaration ()))
    {
      struct ClassDeclaration *other = O_CAST (_other, ClassDeclaration ());
      while (other && other->superclass && self != other)
	{
	  other =
	    (struct ClassDeclaration *) O_CALL (global_scope, lookup,
						other->superclass);
	}
      return self == other;
    }
  else if (o_is_of (_other, InterfaceDeclaration ()))
    {
      struct InterfaceDeclaration *other = O_CAST (_other, InterfaceDeclaration ());
      return O_CALL (other, is_compatible, self);
    }
  else
    {
      return false;
    }
}

O_IMPLEMENT (ClassDeclaration, void *, find_common_base, (void *_self, void *_other))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  struct ClassDeclaration *other = O_CAST (_self, ClassDeclaration ());

  // TODO find common base for interfaces

  while (self && !O_CALL (self, is_compatible, _other))
    {
      if (self->superclass)
	{
	  self =
	    (struct ClassDeclaration *) O_CALL (global_scope, lookup,
						self->superclass);
	}
      else
	{
	  self = NULL;
	}
    }

  return self;
}

O_OBJECT (ClassDeclaration, ObjectTypeDeclaration);
O_OBJECT_METHOD (ClassDeclaration, ctor);
O_OBJECT_METHOD (ClassDeclaration, dtor);
O_OBJECT_METHOD (ClassDeclaration, accept);
O_OBJECT_METHOD (ClassDeclaration, type_check);
O_OBJECT_METHOD (ClassDeclaration, is_compatible);
O_OBJECT_METHOD (ClassDeclaration, find_common_base);
O_END_OBJECT
