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
#include "co2/IScope.h"

O_METHOD_DEF (Scope, void, error_not_found,
	      (void *_self, struct Token * token));

#define ScopeClass_Attr				\
  HashClass_Attr;				\
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
  O_METHOD(IScope, exists);			\
  O_METHOD(Scope, error_not_found)

#define Scope_Attr				\
  Hash_Attr;					\
  ScopeType type;				\
  struct Token * name;				\
  struct IScope * parent

O_CLASS (Scope, Hash);

#endif /* Scope_H */
