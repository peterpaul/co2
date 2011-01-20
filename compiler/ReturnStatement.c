#include "ReturnStatement.h"
#include "FunctionType.h"
#include "TryStatement.h"
#include "CatchStatement.h"
#include "io.h"

#define O_SUPER Statement()

O_IMPLEMENT (ReturnStatement, void *, ctor, (void *_self, va_list * app))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());
  self = O_SUPER->ctor (self, app);
  self->expr = O_BRANCH_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (ReturnStatement, void *, dtor, (void *_self))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());
  O_BRANCH_CALL (self->expr, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ReturnStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());
  O_BRANCH_CALL (self->expr, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (ReturnStatement, void, generate, (void *_self))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());

  fprintf (out, "{\n");
  if (self->expr)
    {
      O_CALL (self->expr->type, generate);
      fprintf (out, " __return_value = ");
      O_CALL (self->expr, generate);
      fprintf (out, ";\n");
    }

  if (self->try_context)
    {
      fprintf (out, "ex_pop ();\n");
      if (self->try_context->finally_clause)
	{
	  fprintf (out, "do_finally;\n");
	}
    }

  struct FunctionType *function_type = o_cast (self->function_context->type, FunctionType ());
  if (function_type->has_var_args)
    {
      fprintf (out, "va_end (ap);\n");
    }

  fprintf (out, "return ");
  if (self->expr)
    {
      fprintf (out, "__return_value");
    }
  fprintf (out, ";\n");
  
  fprintf (out, "}\n");
}

O_IMPLEMENT (ReturnStatement, void, type_check, (void *_self))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());
  O_BRANCH_CALL (self->expr, type_check);

  self->function_context = O_BRANCH_CALL (current_context, find, FunctionDeclaration ());
  struct FunctionType *function_type = o_cast (self->function_context->type, FunctionType ());
  if (self->expr && self->expr->type)
    {
      O_CALL (function_type->return_type, assert_compatible, self->expr->type);
    }

  self->try_context = O_BRANCH_CALL (current_context, find, TryStatement ());
  self->catch_context = O_BRANCH_CALL (current_context, find, CatchStatement ());
}

O_OBJECT (ReturnStatement, Statement);
O_OBJECT_METHOD (ReturnStatement, ctor);
O_OBJECT_METHOD (ReturnStatement, dtor);
O_OBJECT_METHOD (ReturnStatement, accept);
O_OBJECT_METHOD (ReturnStatement, generate);
O_OBJECT_METHOD (ReturnStatement, type_check);
O_END_OBJECT
