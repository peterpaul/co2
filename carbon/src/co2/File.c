#include "co2/File.h"
#include "co2/Path.h"
#include "co2/RefList.h"
#include "co2/Declaration.h"
#include "co2/io.h"

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

O_IMPLEMENT (File, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct File *self = O_CAST (_self, File ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->declarations, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
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

O_OBJECT (File, CompileObject);
O_OBJECT_METHOD (File, ctor);
O_OBJECT_METHOD (File, dtor);
O_OBJECT_METHOD (File, type_check);
O_OBJECT_METHOD (File, optimize);
O_OBJECT_METHOD (File, generate);
O_OBJECT_METHOD (File, accept);
O_END_OBJECT
