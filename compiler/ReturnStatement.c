#include "ReturnStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT(ReturnStatement, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ReturnStatement * self = O_CAST(_self, ReturnStatement());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(ReturnStatement, void *, dtor, (void *_self), (_self))
{
  struct ReturnStatement *self = O_CAST(_self, ReturnStatement());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(ReturnStatement, Statement);
O_OBJECT_METHOD(ReturnStatement, ctor);
O_OBJECT_METHOD(ReturnStatement, dtor);
O_END_OBJECT
