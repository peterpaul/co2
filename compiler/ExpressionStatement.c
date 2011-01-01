#include "ExpressionStatement.h"
#include "io.h"

#define O_SUPER Statement()

O_IMPLEMENT (ExpressionStatement, void *, ctor, (void *_self, va_list * app))
{
  struct ExpressionStatement *self = O_CAST (_self, ExpressionStatement ());
  self = O_SUPER->ctor (self, app);
  self->expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (ExpressionStatement, void *, dtor, (void *_self))
{
  struct ExpressionStatement *self = O_CAST (_self, ExpressionStatement ());
  O_CALL (self->expr, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ExpressionStatement, void, generate, (void *_self))
{
  struct ExpressionStatement *self = O_CAST (_self, ExpressionStatement ());
  O_CALL (self->expr, generate);
  fprintf (out, ";\n");
}

O_IMPLEMENT (ExpressionStatement, void, type_check, (void *_self))
{
  struct ExpressionStatement *self = O_CAST (_self, ExpressionStatement ());
  O_CALL (self->expr, type_check);
}

O_OBJECT (ExpressionStatement, Statement);
O_OBJECT_METHOD (ExpressionStatement, ctor);
O_OBJECT_METHOD (ExpressionStatement, dtor);
O_OBJECT_METHOD (ExpressionStatement, generate);
O_OBJECT_METHOD (ExpressionStatement, type_check);
O_END_OBJECT
