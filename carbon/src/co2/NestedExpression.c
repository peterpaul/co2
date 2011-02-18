#include "co2/NestedExpression.h"
#include "co2/Type.h"
#include "co2/io.h"

#define O_SUPER Expression()

O_IMPLEMENT (NestedExpression, void *, ctor, (void *_self, va_list * app))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  self = O_SUPER->ctor (self, app);
  self->expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (NestedExpression, void *, dtor, (void *_self))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  O_CALL (self->expr, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (NestedExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  O_CALL (self->expr, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (NestedExpression, void, type_check, (void *_self))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  O_CALL (self->expr, type_check);
  self->type = O_CALL (self->expr->type, retain);
}

O_IMPLEMENT (NestedExpression, void, generate, (void *_self))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  fprintf (out, "(");
  O_CALL (self->expr, generate);
  fprintf (out, ")");
}

O_OBJECT (NestedExpression, Expression);
O_OBJECT_METHOD (NestedExpression, ctor);
O_OBJECT_METHOD (NestedExpression, dtor);
O_OBJECT_METHOD (NestedExpression, accept);
O_OBJECT_METHOD (NestedExpression, type_check);
O_OBJECT_METHOD (NestedExpression, generate);
O_END_OBJECT
