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
#ifndef IScope_H
#define IScope_H

#include "co2/Interface.h"
#include "co2/Token.h"
#include "co2/Declaration.h"

typedef enum
{
  GLOBAL_SCOPE,
  VARIABLE_SCOPE,
  CLASS_SCOPE,
  ARGUMENT_SCOPE,
  COMPOUND_SCOPE,
  INTERFACE_SCOPE,
  STRUCT_SCOPE,
  CATCH_SCOPE
} ScopeType;

O_METHOD_DEF (IScope, struct IScope *, get_parent, (void *_self));
O_METHOD_DEF (IScope, struct IScope *, set_parent, (void *_self, void *_parent_scope));
O_METHOD_DEF (IScope, ScopeType, get_type, (void *_self));
O_METHOD_DEF (IScope, struct Declaration *, declare,
	      (void *_self, struct Declaration * decl));
O_METHOD_DEF (IScope, void, leave, (void *_self));
O_METHOD_DEF (IScope, struct Declaration *, lookup_in_this_scope,
	      (void *_self, struct Token * token));
O_METHOD_DEF (IScope, struct Declaration *, lookup,
	      (void *_self, struct Token * token));
O_METHOD_DEF (IScope, struct Declaration *, find_type_in_this_scope,
	      (void *_self, struct Token * token, void *_type));
O_METHOD_DEF (IScope, struct Declaration *, find_type,
	      (void *_self, struct Token * token, void *_type));
O_METHOD_DEF (IScope, bool, exists_in_this_scope,
	      (void *_self, struct Token * token));
O_METHOD_DEF (IScope, bool, exists, (void *_self, struct Token * token));

#define IScopeClass_Attr			\
  InterfaceClass_Attr

#define IScope_Attr				\
  Interface_Attr;				\
  O_METHOD(IScope, get_parent);			\
  O_METHOD(IScope, set_parent);			\
  O_METHOD(IScope, get_type);			\
  O_METHOD(IScope, declare);			\
  O_METHOD(IScope, leave);			\
  O_METHOD(IScope, lookup_in_this_scope);	\
  O_METHOD(IScope, lookup);			\
  O_METHOD(IScope, find_type_in_this_scope);	\
  O_METHOD(IScope, find_type);			\
  O_METHOD(IScope, exists_in_this_scope);	\
  O_METHOD(IScope, exists)

O_CLASS(IScope, Interface);

extern struct IScope *current_scope;
extern struct IScope *global_scope;

#endif /* IScope_H */
