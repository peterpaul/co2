#include "BinaryExpression.h"
#include "Token.h"

#define O_SUPER Expression()

O_IMPLEMENT(BinaryExpression, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct BinaryExpression * self = O_CAST(_self, BinaryExpression());
  self = O_SUPER->ctor(self, app);
  self->operand[0] = O_BRANCH_CAST(va_arg(*app, struct Expression *), Expression());
  self->operator = O_BRANCH_CAST(va_arg(*app, struct Token *), Token());
  self->operand[1] = O_BRANCH_CAST(va_arg(*app, struct Expression *), Expression());
  O_BRANCH_CALL(self->operand[0], retain);
  O_BRANCH_CALL(self->operand[1], retain);
  O_BRANCH_CALL(self->operator, retain);
  return self;
}

O_IMPLEMENT(BinaryExpression, void *, dtor, (void *_self), (_self))
{
  struct BinaryExpression *self = O_CAST(_self, BinaryExpression());
  O_BRANCH_CALL(self->operand[0], release);
  O_BRANCH_CALL(self->operand[1], release);
  O_BRANCH_CALL(self->operator, release);
  return O_SUPER->dtor(self);
}

O_OBJECT(BinaryExpression, Expression);
O_OBJECT_METHOD(BinaryExpression, ctor);
O_OBJECT_METHOD(BinaryExpression, dtor);
O_END_OBJECT
