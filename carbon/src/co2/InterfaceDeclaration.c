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
#include "co2/FunctionDeclaration.h"
#include "co2/FunctionType.h"
#include "co2/PrimitiveType.h"
#include "co2/Statement.h"
#include "co2/io.h"
#include "grammar.h"
#include "co2/ClassDeclaration.h"

#define O_SUPER ObjectTypeDeclaration()

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

static void InterfaceDeclaration_add_member_to_scope(void *_decl, va_list *app)
{
  struct Declaration *decl = O_CAST(_decl, Declaration ());
  struct InterfaceDeclaration *self = O_GET_ARG (InterfaceDeclaration);
  O_CALL (self->member_scope, declare, decl);
  O_CALL (self->members, append, decl);
}

static void InterfaceDeclaration_add_to_scope(void *_token, va_list *app)
{
  struct Token *token = O_CAST(_token, Token ());
  struct InterfaceDeclaration *interface = O_CAST(O_CALL (global_scope, lookup, token), InterfaceDeclaration ());
  struct InterfaceDeclaration *self = O_GET_ARG (InterfaceDeclaration);
  O_CALL (interface->members, map_args, InterfaceDeclaration_add_member_to_scope, self);
}

O_IMPLEMENT (InterfaceDeclaration, void, type_check, (void *_self))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  // Add all methods of super interfaces to this scope.
  O_BRANCH_CALL (self->interfaces, map_args, InterfaceDeclaration_add_to_scope, self);
  O_CALL (self->members, map, CompileObject_type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

static void InterfaceDeclaration_is_compatible_with_class(void *_self, va_list *app)
{
  struct Token *self = O_CAST(_self, Token ());
  struct InterfaceDeclaration *interface = O_CAST(va_arg(*app, struct InterfaceDeclaration*), InterfaceDeclaration ());
  bool *found = va_arg(*app, bool*);
  if (strcmp(self->name->data, interface->name->name->data) == 0) 
    {
      *found = true;
    }
}

static struct ClassDeclaration *InterfaceDeclaration_get_super_class(struct ClassDeclaration *decl)
{
  struct Token *superclass = O_BRANCH_CAST (decl->superclass, Token ());
  if (superclass && O_CALL (global_scope, exists, superclass))
    {
      struct Declaration * superdecl = O_CALL (global_scope, lookup, superclass);
      return O_CAST (superdecl, ClassDeclaration ());
    }
    return NULL;
}

O_IMPLEMENT (InterfaceDeclaration, bool, is_compatible,
	     (void *_self, void *_other))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  struct ObjectTypeDeclaration *other = o_cast (_other, ObjectTypeDeclaration ());
  bool found = false;
  if (o_is_of (other, ClassDeclaration ()))
    {
      struct ClassDeclaration *class_decl = O_CAST (other, ClassDeclaration ());
      do 
	{
	  O_BRANCH_CALL (class_decl->interfaces, map_args, InterfaceDeclaration_is_compatible_with_class, self, &found);
	} 
      while (!found && (class_decl = InterfaceDeclaration_get_super_class (class_decl)));
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
