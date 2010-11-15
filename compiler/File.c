#include "File.h"
#include "Path.h"
#include "RefList.h"
#include "Declaration.h"
#include "io.h"

#define O_SUPER CompileObject()

static void type_check(void *_declaration)
{
  struct Declaration * declaration = O_CAST(_declaration, Declaration());
  O_CALL(declaration, type_check);
}

static void optimize(void *_declaration)
{
}

static void generate(void *_declaration)
{
  struct Declaration * declaration = O_CAST(_declaration, Declaration());
  O_CALL(declaration, generate);
}

O_IMPLEMENT(File, void *, ctor, (void *_self, va_list *app))
{
  struct File * self = O_CAST(_self, File());
  self = O_SUPER->ctor(self, app);
  self->package = O_CAST(va_arg(*app, struct Path *), Path());
  O_CALL(self->package, retain);
  self->import_statements = O_CAST(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->import_statements, retain);
  self->declarations = O_CAST(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->declarations, retain);
  return self;
}

O_IMPLEMENT(File, void *, dtor, (void *_self))
{
  struct File *self = O_CAST(_self, File());
  O_CALL(self->package, release);
  O_CALL(self->import_statements, release);
  O_CALL(self->declarations, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(File, void, type_check, (void *_self))
{
  struct File *self = O_CAST(_self, File());
  current_context = O_CALL_CLASS(Context(), new);
  O_CALL (current_context, retain);
  O_CALL (self->declarations, map, type_check);
  O_CALL (current_context, release);
}

O_IMPLEMENT(File, void, optimize, (void *_self))
{
  struct File *self = O_CAST(_self, File());
  O_CALL (self->declarations, map, optimize);
}

static int File_definition_filter(void *_member)
{
  struct Declaration * member = O_CAST(_member, Declaration());
  return member->include_file != NULL;
}

static int File_declaration_filter(void *_member)
{
  struct Declaration * member = O_CAST(_member, Declaration());
  return member->include_file == NULL;
}

O_IMPLEMENT(File, void, generate, (void *_self))
{
  struct File *self = O_CAST(_self, File());
  struct RefList * definitions = O_CALL(self->declarations, filter, File_definition_filter);
  O_CALL(definitions, retain);
  struct RefList * declarations = O_CALL(self->declarations, filter, File_declaration_filter);
  O_CALL(declarations, retain);

  O_CALL (definitions, map, generate);
  O_CALL (declarations, map, generate);

  O_CALL(definitions, release);
  O_CALL(declarations, release);
}

void parse_import(void *_import, va_list *app)
{
  struct Path * import = o_cast(_import, Path());
  struct String * import_path = O_CALL(import, to_system_path);
  fprintf(stderr, "import: %s\n", import_path->data);
  /*
  struct String * path = O_CALL(base_dir, clone);
  O_CALL(path, append, import_path);
  O_CALL(import_path, delete);
  O_CALL(path, delete);
  */
  new_input(import_path->data);
  yyparse();
}

O_IMPLEMENT(File, void, parse_imports, (void *_self))
{
  struct File *self = O_CAST(_self, File());
  fprintf(stderr, "checking imports...\n");
  if (!self->imported_files)
    {
      self->imported_files = O_CALL_CLASS(RefList(), new, 0, File());
    }
  O_CALL(self->import_statements, map_args, parse_import, self);
}

O_OBJECT(File, CompileObject);
O_OBJECT_METHOD(File, ctor);
O_OBJECT_METHOD(File, dtor);
O_OBJECT_METHOD(File, type_check);
O_OBJECT_METHOD(File, optimize);
O_OBJECT_METHOD(File, generate);
O_OBJECT_METHOD(File, parse_imports);
O_END_OBJECT
