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
#include "co2/File.h"
#include "co2/Path.h"
#include "co2/RefList.h"
#include "co2/Declaration.h"
#include "co2/ClassDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/ObjectType.h"
#include "co2/TypeDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/VariableDeclaration.h"
#include "co2/io.h"

static int Declaration_compare (const void *_decl1, const void *_decl2);
static int TypeDeclaration_compare (const void *_decl1, const void *_decl2);
static bool InterfaceDeclaration_1_depends_on_2 (const struct InterfaceDeclaration * decl1, const struct InterfaceDeclaration * decl2);

#define O_SUPER CompileObject()

struct Hash * file_hash_map = NULL;

struct Hash * get_hash_map ()
{
  if (!file_hash_map)
    {
      file_hash_map = O_CALL_CLASS (Hash (), new);
    }
  return file_hash_map;
}

static void
File_type_check (void *_object)
{
  struct CompileObject *object = O_CAST (_object, CompileObject ());
  if (o_is_of (object, Declaration ()))
    {
      current_context = O_CALL_CLASS (Context (), new);
      O_BRANCH_CALL (current_context, retain);
      O_CALL (object, type_check);
      O_BRANCH_CALL (current_context, release);
      current_context = NULL;
    }
  else
    {
      O_CALL (object, type_check);
    }
}

static void
optimize (void *_object)
{
}

void
accept_all_files (void *_file, va_list *app)
{
  struct File *file = O_CAST (_file, File ());
  struct BaseCompileObjectVisitor *visitor = O_GET_ARG (BaseCompileObjectVisitor);
  O_CALL (file, accept_all_files, visitor);
}

O_IMPLEMENT (File, void *, ctor, (void *_self, va_list * app))
{
  struct File *self = O_CAST (_self, File ());
  self = O_SUPER->ctor (self, app);
  self->name = O_RETAIN_ARG (String);
  self->absolute_path = O_RETAIN_ARG (String);
  self->declarations = O_CALL_CLASS (RefList (), new, 8, Declaration ());
  self->declarations = O_CALL (self->declarations, retain);
  self->file_dependencies = O_CALL_CLASS (RefList (), new, 8, File ());
  self->file_dependencies = O_CALL (self->file_dependencies, retain);
  return self;
}

O_IMPLEMENT (File, void *, dtor, (void *_self))
{
  struct File *self = O_CAST (_self, File ());
  O_CALL (self->name, release);
  O_CALL (self->absolute_path, release);
  O_CALL (self->declarations, release);
  O_CALL (self->file_dependencies, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (File, void, accept_all_files, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct File *self = O_CAST (_self, File ());
  O_CALL (self->file_dependencies, map_args, accept_all_files, visitor);
  O_CALL (self, accept, visitor);
}


O_IMPLEMENT (File, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct File *self = O_CAST (_self, File ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->declarations, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

static bool Token_equals (struct Token * self, struct Token * other)
{
  return strcmp (self->name->data, other->name->data) == 0;
}

static void Token_equals_callback (const void *_self, va_list *app)
{
  struct Token *self = O_CAST (_self, Token ());
  struct Token *interface = O_GET_ARG (Token);
  bool *depends = va_arg (*app, bool *);
  if (Token_equals (self, interface))
    {
      *depends = true;
    }
}

static void Interface_depends_on (const void *_self, va_list *app)
{
  struct Token *self = O_CAST (_self, Token ());
  struct InterfaceDeclaration *decl2 = O_GET_ARG (InterfaceDeclaration);
  bool *depends = va_arg (*app, bool *);
  if (Token_equals (self, decl2->name))
    {
      *depends = true;
    }
  else
    {
      if (O_CALL (global_scope, exists, self))
	{
	  struct Declaration *super_decl = O_BRANCH_CALL (global_scope, lookup, self);
	  if (o_is_of (super_decl, InterfaceDeclaration ()))
	    {
	      if (InterfaceDeclaration_1_depends_on_2 ((struct InterfaceDeclaration *) super_decl, decl2))
		{
		  *depends = true;
		}
	    }
	}
    }
}

static bool InterfaceDeclaration_1_depends_on_2 (const struct InterfaceDeclaration * decl1, const struct InterfaceDeclaration * decl2)
{
  bool depends = false;
  if (decl1->interfaces)
    {
      O_CALL (decl1->interfaces, map_args, Interface_depends_on, decl2, &depends);
    }
  return depends;
}

static int InterfaceDeclaration_compare_Interface (const struct InterfaceDeclaration * decl1, const void *_decl2)
{
  const struct InterfaceDeclaration * decl2 = O_CAST (_decl2, InterfaceDeclaration ());
  if (InterfaceDeclaration_1_depends_on_2 (decl1, decl2))
    {
      return 1;
    }
  if (InterfaceDeclaration_1_depends_on_2 (decl2, decl1))
    {
      return -1;
    }
  return 0;
}

static int InterfaceDeclaration_compare_Class (const struct InterfaceDeclaration * decl1, const void *_decl2)
{
  const struct ClassDeclaration * decl2 = O_CAST (_decl2, ClassDeclaration ());
  bool depends = false;
  O_BRANCH_CALL (decl2->interfaces, map_args, Token_equals_callback, decl1->name, &depends);
  if (depends)
    {
      fprintf (stderr, "%s depends on %s\n", decl2->name->name->data, decl1->name->name->data);
      return -1;
    }
}

static bool ClassDeclaration_1_depends_on_2 (const struct ClassDeclaration * decl1, const struct ClassDeclaration * decl2)
{
  if (decl1->superclass)
    {
      if (Token_equals (decl1->superclass, decl2->name))
	{
	  return true;
	}
      if (O_CALL (global_scope, exists, decl1->superclass))
	{
	  struct Declaration *super_decl = O_BRANCH_CALL (global_scope, lookup, decl1->superclass);
	  if (o_is_of (super_decl, ClassDeclaration ()))
	    {
	      return ClassDeclaration_1_depends_on_2 ((struct ClassDeclaration *) super_decl, decl2);
	    }
	}
    }
  return false;
}

static int ClassDeclaration_compare_Class (const struct ClassDeclaration * decl1, const void *_decl2)
{
  const struct ClassDeclaration * decl2 = O_CAST (_decl2, ClassDeclaration ());
  int result = ClassDeclaration_1_depends_on_2 (decl1, decl2);
  if (result == 0)
    {
      result = -ClassDeclaration_1_depends_on_2 (decl2, decl1);
    }
  return result;
}

static int InterfaceDeclaration_compare (const void *_decl1, const void *_decl2)
{
  const struct InterfaceDeclaration * decl1 = O_CAST (_decl1, InterfaceDeclaration ());
  if (o_is_of (_decl2, InterfaceDeclaration ()))
    {
      return InterfaceDeclaration_compare_Interface (decl1, _decl2);
    }
  else if (o_is_of (_decl2, ClassDeclaration ()))
    {
      return InterfaceDeclaration_compare_Class (decl1, _decl2);
    }
  else if (o_is_of (_decl2, TypeDeclaration ()))
    {
      const struct TypeDeclaration * decl2 = O_CAST (_decl2, TypeDeclaration ());
      if (o_is_of (decl2->type, ObjectType ()))
	{
	  const struct ObjectType * type = O_CAST (decl2->type, ObjectType ());
	  if (!type->decl)
	    {
	      O_CALL (type, type_check);
	    }
	  if (type->decl)
	    {
	      return InterfaceDeclaration_compare (decl1, type->decl);
	    }
	}
    }
  return -1;
}

static int ClassDeclaration_compare (const void *_decl1, const void *_decl2)
{
  const struct ClassDeclaration * decl1 = O_CAST (_decl1, ClassDeclaration ());
  if (o_is_of (_decl2, InterfaceDeclaration ()))
    {
      return -InterfaceDeclaration_compare (_decl2, decl1);
    }
  else if (o_is_of (_decl2, ClassDeclaration ()))
    {
      return ClassDeclaration_compare_Class (decl1, _decl2);
    }
  else if (o_is_of (_decl2, TypeDeclaration ()))
    {
      const struct TypeDeclaration * decl2 = O_CAST (_decl2, TypeDeclaration ());
      if (o_is_of (decl2->type, ObjectType ()))
	{
	  const struct ObjectType * type = O_CAST (decl2->type, ObjectType ());
	  if (!type->decl)
	    {
	      O_CALL (type, type_check);
	    }
	  if (type->decl)
	    {
	      return ClassDeclaration_compare (decl1, type->decl);
	    }
	}
    }
  return -1;
}

static int SimpleDeclaration_compare (const void *_decl1, const void *_decl2)
{
  const struct Declaration * decl1 = O_CAST (_decl1, Declaration ());
  if (strcmp (decl1->name->name->data, "main") == 0)
    {
      return 1;
    }
  if (o_is_of (_decl2, Declaration ()))
    {
      const struct Declaration * decl2 = O_CAST (_decl2, Declaration ());
      if (strcmp (decl2->name->name->data, "main") == 0)
	{
	  return -1;
	}
      else
	{
	  return strcmp (decl1->name->name->data, decl2->name->name->data);
	}
    }
  return -Declaration_compare (_decl2, _decl1);
}

static int Declaration_compare (const void *_decl1, const void *_decl2)
{
  const struct Declaration * decl1 = O_CAST (_decl1, Declaration ());
  if (o_is_of (decl1, InterfaceDeclaration ()))
    {
      return InterfaceDeclaration_compare (decl1, _decl2);
    }
  else if (o_is_of (decl1, ClassDeclaration ()))
    {
      return ClassDeclaration_compare (decl1, _decl2);
    }
  else if (o_is_of (decl1, TypeDeclaration ()))
    {
      return TypeDeclaration_compare (decl1, _decl2);
    }
  else if (o_is_of (decl1, FunctionDeclaration ()))
    {
      return SimpleDeclaration_compare (decl1, _decl2);
    }
  else if (o_is_of (decl1, VariableDeclaration ()))
    {
      return SimpleDeclaration_compare (decl1, _decl2);
    }
  return 0;
}

static int TypeDeclaration_compare (const void *_decl1, const void *_decl2)
{
  const struct TypeDeclaration * decl1 = O_CAST (_decl1, TypeDeclaration ());
  if (o_is_of (decl1->type, ObjectType ()))
    {
      const struct ObjectType * type = O_CAST (decl1->type, ObjectType ());
      if (!type->decl)
	{
	  O_CALL (type, type_check);
	}
      if (type->decl)
	{
	  return Declaration_compare (type->decl, _decl2);
	}
    }
  return 0;
}

static int Declaration_compare_callback (const void *_decl1, const void *_decl2)
{
  const void *decl1 = *(const void **)_decl1;
  const void *decl2 = *(const void **)_decl2;
  return Declaration_compare (decl1, decl2);
}

O_IMPLEMENT (File, void, sort, (void *_self))
{
  struct File *self = O_CAST (_self, File ());

  qsort (self->declarations->data, self->declarations->length, sizeof (void *), Declaration_compare_callback);
}

O_IMPLEMENT (File, void, type_check, (void *_self))
{
  struct File *self = O_CAST (_self, File ());
  O_CALL (self->file_dependencies, map, File_type_check);
  O_CALL (self->declarations, map, File_type_check);
}

O_IMPLEMENT (File, void, optimize, (void *_self))
{
  struct File *self = O_CAST (_self, File ());
  O_CALL (self->declarations, map, optimize);
}

static int
File_definition_filter (void *_member)
{
  struct Declaration *member = O_CAST (_member, Declaration ());
  return member->include_file != NULL;
}

static int
File_declaration_filter (void *_member)
{
  struct Declaration *member = O_CAST (_member, Declaration ());
  return member->include_file == NULL;
}

O_IMPLEMENT (File, void, generate, (void *_self))
{
  struct File *self = O_CAST (_self, File ());
  struct RefList *definitions =
    O_CALL (self->declarations, filter, File_definition_filter);
  O_CALL (definitions, retain);
  struct RefList *declarations =
    O_CALL (self->declarations, filter, File_declaration_filter);
  O_CALL (declarations, retain);

  O_CALL (definitions, map, CompileObject_generate);
  O_CALL (declarations, map, CompileObject_generate);

  O_CALL (definitions, release);
  O_CALL (declarations, release);
}

static void
File_generateDepend_callback (void *_object)
{
  struct File *object = O_CAST (_object, File ());

  O_CALL (object, generateDepend);
}

O_IMPLEMENT (File, void, generateDepend, (void *_self))
{
  struct File *self = O_CAST (_self, File ());

  printf (" \\\n %s", self->absolute_path->data);
  O_CALL (self->file_dependencies, map, File_generateDepend_callback);
}

O_OBJECT (File, CompileObject);
O_OBJECT_METHOD (File, ctor);
O_OBJECT_METHOD (File, dtor);
O_OBJECT_METHOD (File, sort);
O_OBJECT_METHOD (File, type_check);
O_OBJECT_METHOD (File, optimize);
O_OBJECT_METHOD (File, generate);
O_OBJECT_METHOD (File, generateDepend);
O_OBJECT_METHOD (File, accept);
O_OBJECT_METHOD (File, accept_all_files);
O_END_OBJECT
