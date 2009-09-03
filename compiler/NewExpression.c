#include "NewExpression.h"

#define O_SUPER Expression()

O_IMPLEMENT(NewExpression, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct NewExpression * self = O_CAST(_self, NewExpression());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(NewExpression, void *, dtor, (void *_self), (_self))
{
  struct NewExpression *self = O_CAST(_self, NewExpression());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(NewExpression, Expression);
O_OBJECT_METHOD(NewExpression, ctor);
O_OBJECT_METHOD(NewExpression, dtor);
O_END_OBJECT
