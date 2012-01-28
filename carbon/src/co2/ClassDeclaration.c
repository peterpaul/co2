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
#include "co2/InterfaceMethodDefinition.h"
#include "co2/Type.h"
#include "co2/ObjectType.h"
#include "co2/Token.h"
#include "co2/RefList.h"
#include "co2/FunctionType.h"
#include "co2/io.h"
#include "co2/Statement.h"
#include "co2/IScope.h"
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
  O_BRANCH_CALL (self->superclass, accept, visitor);
  O_BRANCH_CALL (self->interfaces, map_args, accept, visitor);
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
      return O_BRANCH_CALL_IF (IScope, O_CALL_IF (IScope, constructor->scope, get_parent),
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
  /* TODO check whether:
   * 1. superclasses already implement interfaces, if so: fail
   * 2. foreach interface, methods already exists in other interface (including interfaces from superclasses), if so: fail
   * 3. class (including superclasses) implements all methods of all interfaces, if not: fail
   */

  void type_check_interfaces (void *_interface_name)
  {
    struct Token * interface_name = O_CAST (_interface_name, Token ());
    void * _interface_decl = O_CALL_IF (IScope, global_scope, lookup, interface_name);
    
    if (o_is_of (_interface_decl, InterfaceDeclaration ()))
      {
	struct InterfaceDeclaration * interface_decl = O_CAST (_interface_decl, InterfaceDeclaration ());

	void type_check_interface_members (void *_if_method_decl)
	{
	  if (o_is_of (_if_method_decl, FunctionDeclaration ()))
	    {
	      struct FunctionDeclaration * if_method_decl = O_CAST (_if_method_decl, FunctionDeclaration ());

	      if (if_method_decl->body)
		{
		  if (!O_CALL_IF (IScope, self->member_scope, exists_in_this_scope, if_method_decl->name))
		    {
		      struct FunctionDeclaration * method_decl = O_CALL_CLASS (FunctionDeclaration (), new, if_method_decl->name, if_method_decl->type, if_method_decl->formal_arguments, NULL);
		      
		      method_decl->implemented_methods = O_CALL_CLASS (RefList (), new, 8, InterfaceMethodDefinition ());
		      struct InterfaceMethodDefinition * imd = O_CALL_CLASS (InterfaceMethodDefinition (), new, interface_name, if_method_decl->name);
		      imd->interface_decl = O_CALL (interface_decl, retain);
		      imd->method_decl = O_CALL (if_method_decl, retain);
		      O_CALL (method_decl->implemented_methods, append, imd);
		      
		      O_CALL_IF (IScope, self->member_scope, declare, (struct Declaration *) method_decl);
		      O_CALL (self->members, append, method_decl);
		    } 
		  else
		    {
		      struct FunctionDeclaration * method_decl = O_CAST (O_CALL_IF (IScope, self->member_scope, lookup_in_this_scope, if_method_decl->name), FunctionDeclaration ());
		      
		      struct InterfaceMethodDefinition * imd = O_CALL_CLASS (InterfaceMethodDefinition (), new, interface_name, if_method_decl->name);
		      imd->interface_decl = O_CALL (interface_decl, retain);
		      imd->method_decl = O_CALL (if_method_decl, retain);
		      O_CALL (method_decl->implemented_methods, append, imd);

		      // O_CALL (self->members, append, method_decl);
		    }
		}
	    }
	  else
	    {
	      struct Declaration * member_decl = O_CAST (_if_method_decl, Declaration ());
	      error (interface_name, "Not a method: '%s'\n", member_decl->name->name->data);
	    }
	}

	O_CALL (interface_decl->members, map, type_check_interface_members);
      }
    else
      {
	error (interface_name, "Not an interface: '%s'\n", interface_name->name->data);
      }
  }

  O_BRANCH_CALL (self->interfaces, map, type_check_interfaces);

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
	    (struct ClassDeclaration *) O_CALL_IF (IScope, global_scope, lookup,
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
	    (struct ClassDeclaration *) O_CALL_IF (IScope, global_scope, lookup,
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
