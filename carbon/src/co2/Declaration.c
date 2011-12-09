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
#include "co2/Declaration.h"
#include "co2/Token.h"
#include "co2/Type.h"
#include "co2/ClassDeclaration.h"
#include "co2/Compiler.h"

#define O_SUPER CompileObject()

void
Declaration_list_set_include_header (void *_decl, va_list * app)
{
  struct Declaration *decl = o_cast (_decl, Declaration ());
  struct Token *header_file = O_GET_ARG (Token);
  O_CALL (decl, set_include_file, header_file);
}

int
Declaration_new_member_filter (void *_member, va_list * app)
{
  struct Declaration *member = O_CAST (_member, Declaration ());
  struct Class *_type = va_arg (*app, struct Class *);
  struct Class *type = O_IS_CLASS (_type);
  struct IScope *parent_scope = O_CALL_IF (IScope, member->scope, get_parent);
  return O_BRANCH_CALL_IF (IScope, parent_scope, find_type,
			member->name, type) == NULL;
}

O_IMPLEMENT (Declaration, void *, ctor, (void *_self, va_list * app))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  self = O_SUPER->ctor (self, app);
  self->name = O_RETAIN_ARG (Token);
  self->file = current_file;
  return self;
}

O_IMPLEMENT (Declaration, void *, dtor, (void *_self))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  O_CALL (self->name, release);
  O_BRANCH_CALL (self->type, release);
  O_BRANCH_CALL (self->include_file, release);
  O_BRANCH_CALL (self->class_decl, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (Declaration, void, set_scope, (void *_self, void *_scope))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  self->scope = o_cast_interface (_scope, IScope ());
}

O_IMPLEMENT (Declaration, void, set_class_decl,
	     (void *_self, void *_class_decl))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  self->class_decl = o_cast (_class_decl, ClassDeclaration ());
  O_CALL (self->class_decl, retain);
}

O_IMPLEMENT (Declaration, void, set_include_file,
	     (void *_self, void *_include_file))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  self->include_file = o_cast (_include_file, Token ());
  O_CALL (self->include_file, retain);
}

O_OBJECT (Declaration, CompileObject);
O_OBJECT_METHOD (Declaration, ctor);
O_OBJECT_METHOD (Declaration, dtor);
O_OBJECT_METHOD (Declaration, set_scope);
O_OBJECT_METHOD (Declaration, set_class_decl);
O_OBJECT_METHOD (Declaration, set_include_file);
O_END_OBJECT
