#include "VariableDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Expression.h"
#include "io.h"

#define O_SUPER Declaration()

O_IMPLEMENT (VariableDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct VariableDeclaration *self = O_CAST (_self, VariableDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->expr = O_BRANCH_CALL (va_arg (*app, struct Expression *), retain);
  return self;
}

O_IMPLEMENT (VariableDeclaration, void, set_type,
	     (void *_self, struct Type * type))
{
  struct VariableDeclaration *self = O_CAST (_self, VariableDeclaration ());
  self->type = O_CALL (type, retain);
}

O_IMPLEMENT (VariableDeclaration, void, generate, (void *_self))
{
  struct VariableDeclaration *self = O_CAST (_self, VariableDeclaration ());
  // don't generate if external definition
  if (self->include_file)
    {
      fprintf (out, "#include ");
      O_CALL (self->include_file, generate);
      fprintf (out, "\n");
      return;
    }
  O_CALL (self->type, generate);
  fprintf (out, " ");
  O_CALL (self->name, generate);
  if (self->expr)
    {
      fprintf (out, "=");
      O_CALL (self->expr, generate);
    }
  fprintf (out, ";\n");
}

O_IMPLEMENT (VariableDeclaration, void, type_check, (void *_self))
{
  struct VariableDeclaration *self = O_CAST (_self, VariableDeclaration ());
  O_CALL (current_context, add, self);
  O_CALL (self->type, type_check);
  if (self->expr)
    {
      O_CALL (self->expr, type_check);
      O_CALL (self->type, assert_compatible, self->expr->type);
    }

  if (self->scope->parent != NULL &&
      O_CALL (self->scope->parent, exists, self->name))
    {
      struct Declaration *first_decl =
	O_CALL (self->scope->parent, lookup, self->name);
      error (self->name, "'%s' already declared at %s:%d\n",
	     self->name->name->data, first_decl->name->file->data,
	     first_decl->name->line);
    }
  O_CALL (current_context, remove_last);
}

O_OBJECT (VariableDeclaration, Declaration);
O_OBJECT_METHOD (VariableDeclaration, ctor);
O_OBJECT_METHOD (VariableDeclaration, set_type);
O_OBJECT_METHOD (VariableDeclaration, generate);
O_OBJECT_METHOD (VariableDeclaration, type_check);
O_END_OBJECT
