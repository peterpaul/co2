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
#include "co2/ConstructorDeclaration.h"
#include "co2/ClassDeclaration.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/Type.h"
#include "co2/RefList.h"
#include "co2/Statement.h"
#include "co2/io.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"
#include "grammar.h"

#define O_SUPER Declaration()

O_IMPLEMENT (ConstructorDeclaration, void *, ctor,
	     (void *_self, va_list * app))
{
  struct ConstructorDeclaration *self =
    O_CAST (_self, ConstructorDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->class_name = O_RETAIN_ARG (Token);
  self->formal_arguments = O_RETAIN_ARG (RefList);
  self->body = O_BRANCH_RETAIN_ARG (Statement);

  struct Declaration * class_decl = O_CALL (global_scope, lookup, self->class_name);
  if (class_decl)
    {
      self->type =
	O_CALL_CLASS (ObjectType (), new, self->class_name, class_decl);
      O_CALL (self->type, retain);
    }

  return self;
}

O_IMPLEMENT (ConstructorDeclaration, void *, dtor, (void *_self))
{
  struct ConstructorDeclaration *self =
    O_CAST (_self, ConstructorDeclaration ());
  O_CALL (self->class_name, release);
  O_CALL (self->formal_arguments, release);
  O_BRANCH_CALL (self->body, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ConstructorDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ConstructorDeclaration *self = O_CAST (_self, ConstructorDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->formal_arguments, map_args, accept, visitor);
  O_BRANCH_CALL (self->body, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (ConstructorDeclaration, void, type_check, (void *_self))
{
  struct ConstructorDeclaration *self =
    O_CAST (_self, ConstructorDeclaration ());
  O_CALL (self->type, type_check);
  O_BRANCH_CALL (current_context, add, self);
  struct Declaration *class_decl =
    O_BRANCH_CALL (current_context, find, ClassDeclaration ());

  if (class_decl == NULL)
    {
      error (self->class_name,
	     "Constructors ('%s') are only allowed in class declarations\n",
	     self->class_name->name->data);
    }
  if (strcmp (class_decl->name->name->data, self->class_name->name->data) !=
      0)
    {
      error (self->class_name, "Constructor should be called '%s'\n",
	     class_decl->name->name->data);
    }

  O_CALL (self->body, type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (ConstructorDeclaration, bool, has_var_args, (void *_self))
{
  struct ConstructorDeclaration *self =
    O_CAST (_self, ConstructorDeclaration ());
  if (self->formal_arguments->length > 0)
    {
      struct ArgumentDeclaration * last_arg = O_CALL (self->formal_arguments, get, self->formal_arguments->length -1);
      if (last_arg->name->type == VA_ARG)
	{
	  return true;
	}
    }
  return false;
}

O_OBJECT (ConstructorDeclaration, Declaration);
O_OBJECT_METHOD (ConstructorDeclaration, ctor);
O_OBJECT_METHOD (ConstructorDeclaration, dtor);
O_OBJECT_METHOD (ConstructorDeclaration, accept);
O_OBJECT_METHOD (ConstructorDeclaration, type_check);
O_OBJECT_METHOD (ConstructorDeclaration, has_var_args);
O_END_OBJECT
