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
#include "co2/ObjectType.h"
#include "co2/Token.h"
#include "co2/ObjectTypeDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/TypeDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/io.h"

#define O_SUPER Type()

O_IMPLEMENT (ObjectType, void *, ctor, (void *_self, va_list * app))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  self = O_SUPER->ctor (self, app);
  self->token = O_RETAIN_ARG (Token);
  self->decl = O_BRANCH_RETAIN_ARG (ObjectTypeDeclaration);
  return self;
}

O_IMPLEMENT (ObjectType, void *, dtor, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  O_CALL (self->token, release);
  O_BRANCH_CALL (self->decl, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ObjectType, void, type_check, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  if (self->decl == NULL)
    {
      self->decl = (struct ObjectTypeDeclaration *) O_CALL_IF (IScope, global_scope, lookup, self->token);
      O_CAST(self->decl, ObjectTypeDeclaration ());
    }
}

O_IMPLEMENT (ObjectType, void, generate, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  if (o_is_of (self->decl, StructDeclaration ()))
    {
      fprintf (out, "struct ");
      O_CALL (self->token, generate);
    }
  else if (o_is_of (self->decl, TypeDeclaration ()))
    {
      struct TypeDeclaration * decl = O_CAST (self->decl, TypeDeclaration ());
      // O_CALL (decl->type, generate);
      O_CALL (self->token, generate);
    }
  else if (o_is_of (self->decl, InterfaceDeclaration ()))
    {
      fprintf (out, "struct Object*");
    }
  else
    {
      fprintf (out, "struct ");
      O_CALL (self->token, generate);
      fprintf (out, "*");
    }
}

O_IMPLEMENT (ObjectType, struct Token *, get_token, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  return self->token;
}

O_IMPLEMENT (ObjectType, struct String *, to_string, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  return O_CALL_CLASS (String (), new, "%s", self->token->name->data);
}

O_IMPLEMENT (ObjectType, bool, is_compatible, (void *_self, void *_other))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  if (O_SUPER->is_compatible (self, _other))
    {
      if (o_is_of (_other, ObjectType ()))
	{
	  struct ObjectType *other = O_CAST (_other, ObjectType ());
	  if (!self->decl)
	    {
	      O_CALL (self, type_check);
	    }
	  if (!other->decl)
	    {
	      O_CALL (other, type_check);
	    }
	  return O_CALL(self->decl, is_compatible, other->decl);
	}
      else
	{
	  /* due to TypeDeclaration */
	  return true;
	}
    }
  else
    {
      struct Type *other = O_CAST (_other, Type ());
      if (O_CALL (other, is_void_ptr))
	{
	  return true;
	}
    }
  return false;
}

O_OBJECT (ObjectType, Type);
O_OBJECT_METHOD (ObjectType, ctor);
O_OBJECT_METHOD (ObjectType, dtor);
O_OBJECT_METHOD (ObjectType, generate);
O_OBJECT_METHOD (ObjectType, type_check);
O_OBJECT_METHOD (ObjectType, get_token);
O_OBJECT_METHOD (ObjectType, to_string);
O_OBJECT_METHOD (ObjectType, is_compatible);
O_END_OBJECT
