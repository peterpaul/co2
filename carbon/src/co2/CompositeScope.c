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
#include "co2/CompositeScope.h"
#include "co2/List.h"

#define O_SUPER Object()

O_IMPLEMENT (CompositeScope, void *, ctor, (void *_self, va_list *app))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct IScope *scope = O_GET_ARG_IF (IScope);
  self = O_SUPER->ctor (self, app);
  self->subscopes = O_CALL_CLASS (List (), new, 8, Object ());
  O_CALL (self->subscopes, append, scope);
  current_scope = self;
  return self;
}

O_IMPLEMENT (CompositeScope, void *, dtor, (void *_self))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  O_CALL (self->subscopes, delete);
  return O_SUPER->dtor (self);  
}

O_IMPLEMENT_IF (CompositeScope, struct IScope *, get_parent, (void *_self), (_self))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct Object *scope = O_CALL (self->subscopes, get, 0);
  return O_CALL_IF (IScope, scope, get_parent);
}

O_IMPLEMENT_IF (CompositeScope, struct IScope *, set_parent, (void *_self, struct IScope *_parent_scope), (_self, _parent_scope))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct Object *scope = O_CALL (self->subscopes, get, 0);
  return O_CALL_IF (IScope, scope, set_parent, _parent_scope);
}

O_IMPLEMENT_IF (CompositeScope, ScopeType, get_type, (void *_self), (_self))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct Object *scope = O_CALL (self->subscopes, get, 0);
  return O_CALL_IF (IScope, scope, get_type);
}

O_IMPLEMENT_IF (CompositeScope, struct Declaration *, declare,
		(void *_self, struct Declaration * decl), (_self, decl))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct Object *scope = O_CALL (self->subscopes, get, 0);
  return O_CALL_IF (IScope, scope, declare, decl);
}

O_IMPLEMENT_IF (CompositeScope, void, leave, (void *_self), (_self))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct Object *scope = O_CALL (self->subscopes, get, 0);
  O_CALL_IF (IScope, scope, leave);
}

O_IMPLEMENT_IF (CompositeScope, struct Declaration *, lookup_in_this_scope,
		(void *_self, struct Token * token), (_self, token))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct Declaration *decl = NULL;
  
  void lookup_in_this_scope_callback (void *_scope)
  {
    struct Object *scope = O_CAST (_scope, Object ());
    if (decl == NULL)
      {
	if (O_CALL_IF (IScope, scope, exists_in_this_scope, token))
	  {
	    decl = O_CALL_IF (IScope, scope, lookup_in_this_scope, token);
	  }
      }
  }

  O_CALL (self->subscopes, map, lookup_in_this_scope_callback);

  if (decl == NULL)
    {
      error (token, "'%s' not declared.\n", token->name->data);
    }
  return decl;
}

O_IMPLEMENT_IF (CompositeScope, struct Declaration *, lookup,
		(void *_self, struct Token * token), (_self, token))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct Declaration *decl = NULL;
  
  void lookup_callback (void *_scope)
  {
    struct Object *scope = O_CAST (_scope, Object ());
    if (decl == NULL)
      {
	if (O_CALL_IF (IScope, scope, exists, token))
	  {
	    decl = O_CALL_IF (IScope, scope, lookup, token);
	  }
      }
  }

  O_CALL (self->subscopes, map, lookup_callback);

  if (decl == NULL)
    {
      error (token, "'%s' not declared.\n", token->name->data);
    }
  return decl;
}

O_IMPLEMENT_IF (CompositeScope, struct Declaration *, find_type_in_this_scope,
		(void *_self, struct Token * token, void *_type), (_self, token, _type))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct Declaration *decl = NULL;
  
  void find_type_in_this_scope_callback (void *_scope)
  {
    struct Object *scope = O_CAST (_scope, Object ());
    if (decl == NULL)
      {
	decl = O_CALL_IF (IScope, scope, find_type_in_this_scope, token, _type);
      }
  }

  O_CALL (self->subscopes, map, find_type_in_this_scope_callback);

  return decl;
}

O_IMPLEMENT_IF (CompositeScope, struct Declaration *, find_type,
		(void *_self, struct Token * token, void *_type), (_self, token, _type))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  struct Declaration *decl = NULL;
  
  void find_type_callback (void *_scope)
  {
    struct Object *scope = O_CAST (_scope, Object ());
    if (decl == NULL)
      {
	decl = O_CALL_IF (IScope, scope, find_type, token, _type);
      }
  }

  O_CALL (self->subscopes, map, find_type_callback);

  return decl;
}

O_IMPLEMENT_IF (CompositeScope, bool, exists_in_this_scope,
		(void *_self, struct Token * token), (_self, token))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  bool result = false;
  
  void exists_in_this_scope_callback (void *_scope)
  {
    struct Object *scope = O_CAST (_scope, Object ());
    if (result == false)
      {
	result = O_CALL_IF (IScope, scope, exists_in_this_scope, token);
      }
  }

  O_CALL (self->subscopes, map, exists_in_this_scope_callback);

  return result;
}

O_IMPLEMENT_IF (CompositeScope, bool, exists, (void *_self, struct Token * token), (_self, token))
{
  struct CompositeScope *self = O_CAST (_self, CompositeScope ());
  bool result = false;
  
  void exists_callback (void *_scope)
  {
    struct Object *scope = O_CAST (_scope, Object ());
    if (result == false)
      {
	result = O_CALL_IF (IScope, scope, exists, token);
      }
  }

  O_CALL (self->subscopes, map, exists_callback);

  return result;
}

O_OBJECT (CompositeScope, Hash);
O_OBJECT_METHOD (CompositeScope, ctor);
O_OBJECT_METHOD (CompositeScope, dtor);
O_OBJECT_METHOD (CompositeScope, get_parent);
O_OBJECT_METHOD (CompositeScope, set_parent);
O_OBJECT_METHOD (CompositeScope, get_type);
O_OBJECT_METHOD (CompositeScope, declare);
O_OBJECT_METHOD (CompositeScope, leave);
// O_OBJECT_METHOD (CompositeScope, error_already_declared);
// O_OBJECT_METHOD (CompositeScope, error_not_found);
O_OBJECT_METHOD (CompositeScope, lookup_in_this_scope);
O_OBJECT_METHOD (CompositeScope, lookup);
O_OBJECT_METHOD (CompositeScope, find_type_in_this_scope);
O_OBJECT_METHOD (CompositeScope, find_type);
O_OBJECT_METHOD (CompositeScope, exists_in_this_scope);
O_OBJECT_METHOD (CompositeScope, exists);
// O_OBJECT_METHOD (CompositeScope, to_string);
O_OBJECT_IF (IScope);
O_OBJECT_IF_METHOD (CompositeScope, get_parent);
O_OBJECT_IF_METHOD (CompositeScope, set_parent);
O_OBJECT_IF_METHOD (CompositeScope, get_type);
O_OBJECT_IF_METHOD (CompositeScope, declare);
O_OBJECT_IF_METHOD (CompositeScope, leave);
O_OBJECT_IF_METHOD (CompositeScope, lookup_in_this_scope);
O_OBJECT_IF_METHOD (CompositeScope, lookup);
O_OBJECT_IF_METHOD (CompositeScope, find_type_in_this_scope);
O_OBJECT_IF_METHOD (CompositeScope, find_type);
O_OBJECT_IF_METHOD (CompositeScope, exists_in_this_scope);
O_OBJECT_IF_METHOD (CompositeScope, exists);
O_OBJECT_IF_END
O_END_OBJECT
