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
#ifndef Scope_H
#define Scope_H

#include "co2/Hash.h"
#include "co2/Declaration.h"
#include "co2/Token.h"

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

O_METHOD_DEF (Scope, struct Declaration *, declare,
	      (void *_self, struct Declaration * decl));
O_METHOD_DEF (Scope, void, leave, (void *_self));
O_METHOD_DEF (Scope, struct Declaration *, lookup_in_this_scope,
	      (void *_self, struct Token * token));
O_METHOD_DEF (Scope, struct Declaration *, lookup,
	      (void *_self, struct Token * token));
O_METHOD_DEF (Scope, struct Declaration *, find_type_in_this_scope,
	      (void *_self, struct Token * token, void *_type));
O_METHOD_DEF (Scope, struct Declaration *, find_type,
	      (void *_self, struct Token * token, void *_type));
O_METHOD_DEF (Scope, bool, exists_in_this_scope,
	      (void *_self, struct Token * token));
O_METHOD_DEF (Scope, bool, exists, (void *_self, struct Token * token));
O_METHOD_DEF (Scope, void, error_not_found,
	      (void *_self, struct Token * token));

#define ScopeClass_Attr				\
  HashClass_Attr;				\
  O_METHOD(Scope, declare);			\
  O_METHOD(Scope, leave);			\
  O_METHOD(Scope, lookup_in_this_scope);	\
  O_METHOD(Scope, lookup);			\
  O_METHOD(Scope, find_type_in_this_scope);	\
  O_METHOD(Scope, find_type);			\
  O_METHOD(Scope, exists_in_this_scope);	\
  O_METHOD(Scope, exists);			\
  O_METHOD(Scope, error_not_found)

#define Scope_Attr				\
  Hash_Attr;					\
  ScopeType type;				\
  struct Token * name;				\
  struct Scope * parent

O_CLASS (Scope, Hash);

extern struct Scope *current_scope;
extern struct Scope *global_scope;

#endif /* Scope_H */
