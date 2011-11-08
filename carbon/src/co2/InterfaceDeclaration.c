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
#include "co2/InterfaceDeclaration.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/CompositeScope.h"
#include "co2/FunctionDeclaration.h"
#include "co2/FunctionType.h"
#include "co2/PrimitiveType.h"
#include "co2/Statement.h"
#include "co2/io.h"
#include "grammar.h"
#include "co2/ClassDeclaration.h"
#include "co2/List.h"

#define O_SUPER ObjectTypeDeclaration ()

O_IMPLEMENT (InterfaceDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->interfaces = O_BRANCH_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT (InterfaceDeclaration, void *, dtor, (void *_self))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  O_BRANCH_CALL (self->interfaces, release);
  O_BRANCH_CALL (self->members, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (InterfaceDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->members, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (InterfaceDeclaration, void, type_check, (void *_self))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());

  O_BRANCH_CALL (current_context, add, self);

  void add_to_scope (void *_token)
  {
    struct Token *token = O_CAST (_token, Token ());
    struct InterfaceDeclaration *interface = O_CAST (O_CALL_IF (IScope, global_scope, lookup, token), InterfaceDeclaration ());

    struct CompositeScope *member_scope = O_CAST (self->member_scope, CompositeScope ());
    O_CALL (member_scope->subscopes, append, interface->member_scope);

    // TODO only add if not already existing in implemented_methods
    // TODO do this only while generating source code
    void add_member_to_scope (void *_decl)
    {
      struct Declaration *decl = O_CAST (_decl, Declaration ());
      O_CALL (self->members, append, decl);
    }
    
    O_CALL (interface->members, map, add_member_to_scope);
  }
  
  // Add all methods of super interfaces to this scope.
  O_BRANCH_CALL (self->interfaces, map, add_to_scope);
  O_CALL (self->members, map, CompileObject_type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (InterfaceDeclaration, bool, is_compatible,
	     (void *_self, void *_other))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  struct ObjectTypeDeclaration *other = o_cast (_other, ObjectTypeDeclaration ());
  bool found = false;

  void is_compatible_with_class (void *_self)
  {
    struct Token *token = O_CAST (_self, Token ());
    if (strcmp (token->name->data, self->name->name->data) == 0) 
      {
	found = true;
      }
  }

  struct ClassDeclaration *get_super_class (struct ClassDeclaration *decl)
  {
    struct Token *superclass = O_BRANCH_CAST (decl->superclass, Token ());
    if (superclass && O_CALL_IF (IScope, global_scope, exists, superclass))
      {
	struct Declaration * superdecl = O_CALL_IF (IScope, global_scope, lookup, superclass);
	return O_CAST (superdecl, ClassDeclaration ());
      }
    return NULL;
  }
  
  if (o_is_of (other, ClassDeclaration ()))
    {
      struct ClassDeclaration *class_decl = O_CAST (other, ClassDeclaration ());
      do 
	{
	  O_BRANCH_CALL (class_decl->interfaces, map, is_compatible_with_class);
	} 
      while (!found && (class_decl = get_super_class (class_decl)));
      return found;
    }
  else
    {
      return strcmp (self->name->name->data, other->name->name->data) == 0;
    }
}

O_OBJECT (InterfaceDeclaration, ObjectTypeDeclaration);
O_OBJECT_METHOD (InterfaceDeclaration, ctor);
O_OBJECT_METHOD (InterfaceDeclaration, dtor);
O_OBJECT_METHOD (InterfaceDeclaration, accept);
O_OBJECT_METHOD (InterfaceDeclaration, type_check);
O_OBJECT_METHOD (InterfaceDeclaration, is_compatible);
O_END_OBJECT

bool interface_implements (struct InterfaceDeclaration *first,
			   struct InterfaceDeclaration *second)
{
  if (second == first)
    {
      return true;
    }
  bool implements = false;
  void interface_implements_callback (struct Token * if_name)
  {
    struct Declaration * decl = O_CALL_IF (IScope, global_scope, lookup, if_name);
    if (!implements && decl && o_is_of (decl, InterfaceDeclaration ()))
      {
	implements = interface_implements (O_CAST (decl, InterfaceDeclaration ()), second);
      }
  }
  O_BRANCH_CALL (first->interfaces, map, interface_implements_callback);
  return implements;
}
