#include "TokenExpression.h"

#define O_SUPER Expression()

O_IMPLEMENT(TokenExpression, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct TokenExpression * self = O_CAST(_self, TokenExpression());
  self = O_SUPER->ctor(self, app);
  self->token = O_CAST(va_arg(*app, struct Token *), Token());
  O_CALL(self->token, retain);
  return self;
}

O_IMPLEMENT(TokenExpression, void *, dtor, (void *_self), (_self))
{
  struct TokenExpression *self = O_CAST(_self, TokenExpression());
  O_CALL(self->token, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(TokenExpression, void, generate, (void *_self), (_self))
{
  struct TokenExpression *self = O_CAST(_self, TokenExpression());
  O_CALL(self->token, generate);
}

O_OBJECT(TokenExpression, Expression);
O_OBJECT_METHOD(TokenExpression, ctor);
O_OBJECT_METHOD(TokenExpression, dtor);
O_OBJECT_METHOD(TokenExpression, generate);
O_END_OBJECT
