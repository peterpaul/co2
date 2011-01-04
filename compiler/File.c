#include "File.h"
#include "Path.h"
#include "RefList.h"
#include "Declaration.h"
#include "io.h"

#define O_SUPER CompileObject()

static void
type_check (void *_declaration)
{
  struct Declaration *declaration = O_CAST (_declaration, Declaration ());
  O_CALL (declaration, type_check);
}

static void
optimize (void *_declaration)
{
}

static void
generate (void *_declaration)
{
  struct Declaration *declaration = O_CAST (_declaration, Declaration ());
  O_CALL (declaration, generate);
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
  printf ("deleting File: %s\n", self->name->data);
  O_CALL (self->name, release);
  O_CALL (self->absolute_path, release);
  O_CALL (self->declarations, release);
  O_CALL (self->file_dependencies, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (File, void, type_check, (void *_self))
{
  struct File *self = O_CAST (_self, File ());
  current_context = O_CALL_CLASS (Context (), new);
  O_CALL (current_context, retain);
  O_CALL (self->declarations, map, type_check);
  O_CALL (current_context, release);
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

  O_CALL (definitions, map, generate);
  O_CALL (declarations, map, generate);

  O_CALL (definitions, release);
  O_CALL (declarations, release);
}

O_OBJECT (File, CompileObject);
O_OBJECT_METHOD (File, ctor);
O_OBJECT_METHOD (File, dtor);
O_OBJECT_METHOD (File, type_check);
O_OBJECT_METHOD (File, optimize);
O_OBJECT_METHOD (File, generate);
O_END_OBJECT
