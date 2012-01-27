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
#include "co2/GenerateSourceIncludesVisitor.h"
#include "co2/CompileObject.h"
#include "co2/ClassDeclaration.h"
#include "co2/VariableDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/TypeDeclaration.h"
#include "co2/IsOfExpression.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"
#include "co2/io.h"
#include "co2/Hash.h"
#include "co2/Compiler.h"
#include "co2/TokenExpression.h"

#define O_SUPER BaseCompileObjectVisitor()

O_IMPLEMENT(GenerateSourceIncludesVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct GenerateSourceIncludesVisitor * self = O_CAST(_self, GenerateSourceIncludesVisitor());
  self = O_SUPER->ctor(self, app);
  self->fp = va_arg (*app, FILE *);
  current_context = O_CALL_CLASS (Context (), new);
  O_BRANCH_CALL (current_context, retain);

  self->map = O_CALL_CLASS (Hash(), new);
  return self;
}

O_IMPLEMENT(GenerateSourceIncludesVisitor, void *, dtor, (void *_self))
{
  struct GenerateSourceIncludesVisitor *self = O_CAST(_self, GenerateSourceIncludesVisitor());
  O_BRANCH_CALL (current_context, release);
  current_context = NULL;
  O_CALL (self->map, delete);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST (_self, GenerateSourceIncludesVisitor ());
  struct Declaration *self = O_CAST (_object, Declaration ());
  if (self->include_file)
    {
      O_CALL(visitor->map, set, self->include_file->name->data, self->include_file->name);
    }
  if (self->file && self->file != main_file)
    {
      struct String * filename = O_CALL_CLASS (String (), new, "\"%s.h\"", self->file->name->data);
      O_CALL (filename, retain);
      O_CALL (visitor->map, set, filename->data, filename);
    }
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitTokenExpression, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
  struct TokenExpression *self = O_CAST (_object, TokenExpression ());

  if (self->decl && self->decl->file && self->decl->file != main_file)
    {
      struct String * filename = O_CALL_CLASS (String (), new, "\"%s.h\"", self->decl->file->name->data);
      O_CALL (filename, retain);
      O_CALL (visitor->map, set, filename->data, filename);
    }

  if (self->type && o_is_of (self->type, ObjectType ()))
    {
      O_CALL (visitor, visitObjectType, self->type);
    }
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitObjectType, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
  struct ObjectType *self = O_CAST (_object, ObjectType ());

  if (self->decl && self->decl->file && self->decl->file != main_file)
    {
      struct String * filename = O_CALL_CLASS (String (), new, "\"%s.h\"", self->decl->file->name->data);
      O_CALL (filename, retain);
      O_CALL (visitor->map, set, filename->data, filename);
    }
}

O_IMPLEMENT_IF(GenerateSourceIncludesVisitor, void, visitFile, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceIncludesVisitor *visitor = O_CAST(_self, GenerateSourceIncludesVisitor());
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

O_OBJECT(GenerateSourceIncludesVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(GenerateSourceIncludesVisitor, ctor);
O_OBJECT_METHOD(GenerateSourceIncludesVisitor, dtor);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitDeclaration);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitTokenExpression);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitObjectType);
O_OBJECT_METHOD (GenerateSourceIncludesVisitor, visitFile);
O_END_OBJECT
