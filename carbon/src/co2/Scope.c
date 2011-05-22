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
#include "co2/Scope.h"
#include "co2/Token.h"

#define O_SUPER Hash()

struct Scope *current_scope = NULL;
struct Scope *global_scope = NULL;

O_IMPLEMENT (Scope, void *, ctor, (void *_self, va_list * app))
{
  struct Scope *self = O_CAST (_self, Scope ());
  self = O_SUPER->ctor (self, app);
  self->type = va_arg (*app, ScopeType);
  self->name = O_BRANCH_RETAIN_ARG (Token);
  self->parent = current_scope;
  current_scope = self;
  return self;
}

O_IMPLEMENT (Scope, void *, dtor, (void *_self))
{
  struct Scope *self = O_CAST (_self, Scope ());
  O_BRANCH_CALL (self->name, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (Scope, struct Declaration *, declare,
	     (void *_self, struct Declaration * decl))
{
  struct Scope *self = O_CAST (_self, Scope ());
  O_CALL (self, add, decl->name->name->data, decl);
  O_CALL (decl, set_scope, self);
  return decl;
}

O_IMPLEMENT (Scope, void, leave, (void *_self))
{
  struct Scope *self = O_CAST (_self, Scope ());
  current_scope = self->parent;
}

O_IMPLEMENT (Scope, void, error_already_declared,
	     (void *_self, const char *key, const void *_item))
{
  struct Scope *self = O_CAST (_self, Scope ());
  struct Declaration *decl = o_cast (_item, Declaration ());
  error (decl->name, "'%s' already declared.\n", key);
}

O_IMPLEMENT (Scope, struct Declaration *, lookup_in_this_scope,
	     (void *_self, struct Token * token))
{
  struct Scope *self = O_CAST (_self, Scope ());
  return O_CALL (self, get, token->name->data);
}

O_IMPLEMENT (Scope, struct Declaration *, lookup,
	     (void *_self, struct Token * token))
{
  struct Scope *self = O_CAST (_self, Scope ());
  struct Declaration *result = O_CALL (self, lookup_in_this_scope, token);
  if (result == NULL)
    {
      if (self->parent == NULL)
	{
	  O_CALL (self, error_not_found, token);
	}
      else
	{
	  result = O_CALL (self->parent, lookup, token);
	}
    }
  return result;
}

O_IMPLEMENT (Scope, struct Declaration *, find_type_in_this_scope,
	     (void *_self, struct Token * token, void *_type))
{
  struct Scope *self = O_CAST (_self, Scope ());
  struct Class *type = O_IS_CLASS (_type);
  struct Declaration *result = O_CALL (self, get, token->name->data);
  if (result != NULL && o_is_of (result, type))
    {
      return result;
    }
  else
    {
      return NULL;
    }
}

O_IMPLEMENT (Scope, struct Declaration *, find_type,
	     (void *_self, struct Token * token, void *_type))
{
  struct Scope *self = O_CAST (_self, Scope ());
  struct Class *type = O_IS_CLASS (_type);
  struct Declaration *result =
    O_CALL (self, find_type_in_this_scope, token, type);
  if (result == NULL)
    {
      if (self->parent != NULL)
	{
	  result = O_CALL (self->parent, find_type, token, type);
	}
    }
  return result;
}

O_IMPLEMENT (Scope, bool, exists_in_this_scope,
	     (void *_self, struct Token * token))
{
  struct Scope *self = O_CAST (_self, Scope ());
  return O_CALL (self, get, token->name->data) != NULL;
}

O_IMPLEMENT (Scope, bool, exists, (void *_self, struct Token * token))
{
  struct Scope *self = O_CAST (_self, Scope ());
  bool result = O_CALL (self, exists_in_this_scope, token);
  if (result == false && self->parent != NULL)
    {
      return O_CALL (self->parent, exists, token);
    }
  return result;
}

O_IMPLEMENT (Scope, void, error_not_found,
	     (void *_self, struct Token * token))
{
  struct Scope *self = O_CAST (_self, Scope ());
  error (token, "'%s' not declared.\n", token->name->data);
}

O_IMPLEMENT (Scope, struct String *, to_string, (void *_self))
{
  struct Scope *self = O_CAST (_self, Scope ());
  struct String *result = NULL;
  if (self->parent)
    {
      result = O_CALL (self->parent, to_string);
      O_CALL(result, append_str, "<-");
      if (self->name)
	{
	  O_CALL (result, append, self->name->name);
	}
      else
	{
	  struct String *super = O_SUPER->to_string (self);
	  O_CALL (result, append, super);
	  O_CALL (super, delete);
	}
    }
  else
    {
      if (self->name)
	{
	  result = O_CALL_CLASS (String (), new, "%s", self->name->name->data); 
	}
      else
	{
	  result = O_SUPER->to_string (self);
	}
    }
  return result;
}

O_OBJECT (Scope, Hash);
O_OBJECT_METHOD (Scope, ctor);
O_OBJECT_METHOD (Scope, dtor);
O_OBJECT_METHOD (Scope, declare);
O_OBJECT_METHOD (Scope, leave);
O_OBJECT_METHOD (Scope, error_already_declared);
O_OBJECT_METHOD (Scope, error_not_found);
O_OBJECT_METHOD (Scope, lookup_in_this_scope);
O_OBJECT_METHOD (Scope, lookup);
O_OBJECT_METHOD (Scope, find_type_in_this_scope);
O_OBJECT_METHOD (Scope, find_type);
O_OBJECT_METHOD (Scope, exists_in_this_scope);
O_OBJECT_METHOD (Scope, exists);
O_OBJECT_METHOD (Scope, to_string);
O_END_OBJECT
