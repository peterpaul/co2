#include "UnaryExpression.h"
#include "Token.h"
#include "io.h"

#define O_SUPER Expression()

O_IMPLEMENT(UnaryExpression, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct UnaryExpression * self = O_CAST(_self, UnaryExpression());
  self = O_SUPER->ctor(self, app);
  self->operator = O_BRANCH_CAST(va_arg(*app, struct Token *), Token());
  self->operand = O_BRANCH_CAST(va_arg(*app, struct Expression *), Expression());
  O_BRANCH_CALL(self->operator, retain);
  O_BRANCH_CALL(self->operand, retain);
  return self;
}

O_IMPLEMENT(UnaryExpression, void *, dtor, (void *_self), (_self))
{
  struct UnaryExpression *self = O_CAST(_self, UnaryExpression());
  O_BRANCH_CALL(self->operator, release);
  O_BRANCH_CALL(self->operand, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(UnaryExpression, void, generate, (void *_self), (self))
{
  struct UnaryExpression *self = O_CAST(_self, UnaryExpression());
  fprintf(out, "%s ", self->operator->name->data);
  O_CALL(self->operand, generate);
}

O_OBJECT(UnaryExpression, Expression);
O_OBJECT_METHOD(UnaryExpression, ctor);
O_OBJECT_METHOD(UnaryExpression, dtor);
O_OBJECT_METHOD(UnaryExpression, generate);
O_END_OBJECT
