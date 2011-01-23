#include "co2/UnaryExpression.h"
#include "co2/Token.h"
#include "co2/io.h"
#include "co2/Type.h"

#define O_SUPER Expression()

O_IMPLEMENT (UnaryExpression, void *, ctor, (void *_self, va_list * app))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  self = O_SUPER->ctor (self, app);
  self->operator = O_BRANCH_RETAIN_ARG (Token);
  self->operand = O_BRANCH_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (UnaryExpression, void *, dtor, (void *_self))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  O_BRANCH_CALL (self->operator, release);
  O_BRANCH_CALL (self->operand, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (UnaryExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  O_BRANCH_CALL (self->operator, accept, visitor);
  O_BRANCH_CALL (self->operand, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (UnaryExpression, void, type_check, (void *_self))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  O_CALL (self->operand, type_check);
  self->type = O_CALL(self->operand->type, retain);
}

O_IMPLEMENT (UnaryExpression, void, generate, (void *_self))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  O_CALL (self->operator, generate);
  fprintf (out, " ");
  O_CALL (self->operand, generate);
}

O_OBJECT (UnaryExpression, Expression);
O_OBJECT_METHOD (UnaryExpression, ctor);
O_OBJECT_METHOD (UnaryExpression, dtor);
O_OBJECT_METHOD (UnaryExpression, accept);
O_OBJECT_METHOD (UnaryExpression, generate);
O_OBJECT_METHOD (UnaryExpression, type_check);
O_END_OBJECT
