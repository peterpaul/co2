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
#include "co2/GenerateHeaderIncludesVisitor.h"
#include "co2/CompileObject.h"
#include "co2/ClassDeclaration.h"
#include "co2/VariableDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/TypeDeclaration.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"
#include "co2/io.h"
#include "co2/Hash.h"
#include "co2/Compiler.h"

#define O_SUPER BaseCompileObjectVisitor()

O_IMPLEMENT(GenerateHeaderIncludesVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct GenerateHeaderIncludesVisitor * self = O_CAST(_self, GenerateHeaderIncludesVisitor());
  self = O_SUPER->ctor(self, app);
  self->fp = va_arg (*app, FILE *);
  current_context = O_CALL_CLASS (Context (), new);
  O_BRANCH_CALL (current_context, retain);

  self->map = O_CALL_CLASS (Hash(), new);
  return self;
}

O_IMPLEMENT(GenerateHeaderIncludesVisitor, void *, dtor, (void *_self))
{
  struct GenerateHeaderIncludesVisitor *self = O_CAST(_self, GenerateHeaderIncludesVisitor());
  O_BRANCH_CALL (current_context, release);
  current_context = NULL;
  O_CALL (self->map, delete);
  return O_SUPER->dtor(self);
}

void handle_interface (void *_token, va_list *app)
{
  struct Token * token = O_CAST (_token, Token ());
  struct Declaration * decl = O_CALL_IF (IScope, global_scope, lookup, token);
  struct GenerateHeaderIncludesVisitor * visitor = O_GET_ARG (GenerateHeaderIncludesVisitor);
  if (decl)
    {
      if (decl->file && decl->file != main_file)
	{
	  struct String * filename = O_CALL_CLASS (String (), new, "\"%s.h\"", decl->file->name->data);
	  O_CALL (filename, retain);
	  O_CALL (visitor->map, set, filename->data, filename);
	}
    }
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct ClassDeclaration *self = O_CAST(_object, ClassDeclaration ());
  
  if (self->superclass) 
    {
      struct Declaration *super = O_CALL_IF (IScope, global_scope, lookup, self->superclass);
      if (super->file && super->file != main_file)
	{
	  struct String * filename = O_CALL_CLASS (String (), new, "\"%s.h\"", super->file->name->data);
	  O_CALL (filename, retain);
	  O_CALL (visitor->map, set, filename->data, filename);
	}
    }
  O_BRANCH_CALL (self->interfaces, map_args, handle_interface, visitor);
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST (_self, GenerateHeaderIncludesVisitor ());
  struct Declaration *self = O_CAST (_object, Declaration ());
  if (self->include_file)
    {
      O_CALL(visitor->map, set, self->include_file->name->data, self->include_file->name);
      return;
    }
  if (self->scope && O_CALL_IF (IScope, self->scope, get_type) != GLOBAL_SCOPE)
    {
      // only generate global declarations
      return;
    }
  O_SUPER->visitDeclaration(visitor, self);
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct InterfaceDeclaration *self = O_CAST (_object, InterfaceDeclaration ());
  O_BRANCH_CALL (self->interfaces, map_args, handle_interface, visitor);
}

O_IMPLEMENT_IF(GenerateHeaderIncludesVisitor, void, visitFile, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderIncludesVisitor *visitor = O_CAST(_self, GenerateHeaderIncludesVisitor());
  struct File *self = O_CAST (_object, File ());

  struct Hash *map = visitor->map;
  int i;
  for (i = 0; i < HASH_SIZE; i++)
    {
      struct Tuple *tuple = map->map[i];
      while (tuple)
	{
	  struct String *value = O_CAST (tuple->value, String ());
	  fprintf (out, "#include %s\n", value->data);
	  tuple = tuple->next;
	}
    }
}

O_OBJECT(GenerateHeaderIncludesVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(GenerateHeaderIncludesVisitor, ctor);
O_OBJECT_METHOD(GenerateHeaderIncludesVisitor, dtor);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitClassDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (GenerateHeaderIncludesVisitor, visitFile);
O_OBJECT_IF(CompileObjectVisitor);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitClassDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitInterfaceDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderIncludesVisitor, visitFile);
O_OBJECT_IF_END
O_END_OBJECT
