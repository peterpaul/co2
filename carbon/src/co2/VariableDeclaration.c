#include "co2/VariableDeclaration.h"
#include "co2/Type.h"
#include "co2/RefList.h"
#include "co2/Expression.h"
#include "co2/FunctionType.h"
#include "co2/io.h"

#define O_SUPER Declaration()

O_IMPLEMENT (VariableDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct VariableDeclaration *self = O_CAST (_self, VariableDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->expr = O_BRANCH_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (VariableDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct VariableDeclaration *self = O_CAST (_self, VariableDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->type, accept, visitor);
  O_BRANCH_CALL (self->expr, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
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
  if (o_is_of (self->type, FunctionType ()))
    {
      struct FunctionType * function_type = O_CAST (self->type, FunctionType ());
      O_CALL (function_type, generate_named, self->name);
    }
  else
    {
      O_CALL (self->type, generate);
      fprintf (out, " ");
      O_CALL (self->name, generate);
    }
  if (self->expr)
    {
      fprintf (out, " = ");
      O_CALL (self->expr, generate);
    }
  fprintf (out, ";\n");
}

O_IMPLEMENT (VariableDeclaration, void, type_check, (void *_self))
{
  struct VariableDeclaration *self = O_CAST (_self, VariableDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->type, type_check);
  if (self->expr)
    {
      O_CALL (self->expr, type_check);
      if (!self->expr->type)
	{
	  return;
	}
      O_CALL (self->type, assert_compatible, self->expr->type);
    }

  if (self->scope->parent != NULL &&
      O_CALL (self->scope->parent, exists, self->name))
    {
      struct Declaration *first_decl =
	O_CALL (self->scope->parent, lookup, self->name);
      if (self->scope->type == first_decl->scope->type && first_decl->scope->type == CLASS_SCOPE)
	{
	  error (self->name, "'%s' already declared at %s:%d\n",
		 self->name->name->data, first_decl->name->file->name->data,
		 first_decl->name->line);
	}
    }
  O_BRANCH_CALL (current_context, remove_last);
}

O_OBJECT (VariableDeclaration, Declaration);
O_OBJECT_METHOD (VariableDeclaration, ctor);
O_OBJECT_METHOD (VariableDeclaration, accept);
O_OBJECT_METHOD (VariableDeclaration, set_type);
O_OBJECT_METHOD (VariableDeclaration, generate);
O_OBJECT_METHOD (VariableDeclaration, type_check);
O_END_OBJECT
