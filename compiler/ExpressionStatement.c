#include "ExpressionStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT(ExpressionStatement, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ExpressionStatement * self = O_CAST(_self, ExpressionStatement());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(ExpressionStatement, void *, dtor, (void *_self), (_self))
{
  struct ExpressionStatement *self = O_CAST(_self, ExpressionStatement());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(ExpressionStatement, Statement);
O_OBJECT_METHOD(ExpressionStatement, ctor);
O_OBJECT_METHOD(ExpressionStatement, dtor);
O_END_OBJECT
