#include "co2/ConditionalExpression.h"
#include "co2/Type.h"
#include "co2/io.h"

#define O_SUPER Expression()

O_IMPLEMENT(ConditionalExpression, void *, ctor, (void *_self, va_list *app))
{
  struct ConditionalExpression * self = O_CAST(_self, ConditionalExpression());
  self = O_SUPER->ctor(self, app);
  self->condition = O_RETAIN_ARG (Expression);
  self->then_expr = O_RETAIN_ARG (Expression);
  self->else_expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT(ConditionalExpression, void *, dtor, (void *_self))
{
  struct ConditionalExpression *self = O_CAST(_self, ConditionalExpression());
  O_CALL (self->condition, release);
  O_CALL (self->then_expr, release);
  O_CALL (self->else_expr, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ConditionalExpression, void, type_check, (void *_self))
{
  struct ConditionalExpression *self = O_CAST(_self, ConditionalExpression());
  O_CALL (self->condition, type_check);
  O_CALL (self->then_expr, type_check);
  O_CALL (self->else_expr, type_check);
  bool then_compat_else = O_CALL (self->then_expr->type, is_compatible, self->else_expr->type);
  bool else_compat_then = O_CALL (self->else_expr->type, is_compatible, self->then_expr->type);
  if (then_compat_else)
    {
      self->type = O_CALL (self->then_expr->type, retain);
    }
  else if (else_compat_then)
    {
      self->type = O_CALL (self->else_expr->type, retain);
    }
  else
    {
      O_CALL (self->then_expr->type, assert_compatible, self->else_expr->type);
      O_CALL (self->else_expr->type, assert_compatible, self->then_expr->type);
    }
}

O_IMPLEMENT(ConditionalExpression, void, generate, (void *_self))
{
  struct ConditionalExpression *self = O_CAST(_self, ConditionalExpression());
  O_CALL (self->condition, generate);
  fprintf (out, "?");
  O_CALL (self->then_expr, generate);
  fprintf (out, ":");
  O_CALL (self->else_expr, generate);
}

O_OBJECT(ConditionalExpression, Expression);
O_OBJECT_METHOD(ConditionalExpression, ctor);
O_OBJECT_METHOD(ConditionalExpression, dtor);
O_OBJECT_METHOD(ConditionalExpression, type_check);
O_OBJECT_METHOD(ConditionalExpression, generate);
O_END_OBJECT
