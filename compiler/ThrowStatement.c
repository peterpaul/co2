#include "ThrowStatement.h"
#include "TryStatement.h"
#include "CatchStatement.h"
#include "io.h"

#define O_SUPER Statement()

O_IMPLEMENT(ThrowStatement, void *, ctor, (void *_self, va_list *app))
{
  struct ThrowStatement * self = O_CAST(_self, ThrowStatement());
  self = O_SUPER->ctor(self, app);
  self->expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT(ThrowStatement, void *, dtor, (void *_self))
{
  struct ThrowStatement *self = O_CAST(_self, ThrowStatement());
  O_CALL (self->expr, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (ThrowStatement, void, accept, (void *_self, struct CompileObjectVisitor *visitor))
{
  struct ThrowStatement *self = O_CAST (_self, ThrowStatement ());
  O_BRANCH_CALL (self->expr, accept, visitor);
  O_CALL_IF (CompileObjectVisitor, visitor, visit, self);
}

O_IMPLEMENT(ThrowStatement, void, type_check, (void *_self))
{
  struct ThrowStatement *self = O_CAST(_self, ThrowStatement());
  self->catch_context = O_CALL (current_context, find, CatchStatement ());
  self->try_context = O_CALL (current_context, find, TryStatement ());
  O_CALL (self->expr, type_check);
}

O_IMPLEMENT(ThrowStatement, void, generate, (void *_self))
{
  struct ThrowStatement *self = O_CAST(_self, ThrowStatement());
  if (self->catch_context)
    {
      fprintf (out, "ex_pop ();\n");
      if (self->try_context->finally_clause)
	{
	  fprintf (out, "do_finally;\n");
	}
    }
  fprintf (out, "throw (1, ");
  O_CALL (self->expr, generate);
  fprintf (out, ");\n");
}

O_OBJECT(ThrowStatement, Statement);
O_OBJECT_METHOD(ThrowStatement, ctor);
O_OBJECT_METHOD(ThrowStatement, dtor);
O_OBJECT_METHOD(ThrowStatement, accept);
O_OBJECT_METHOD(ThrowStatement, type_check);
O_OBJECT_METHOD(ThrowStatement, generate);
O_END_OBJECT
