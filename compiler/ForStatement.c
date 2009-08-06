#include "ForStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT(ForStatement, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ForStatement * self = O_CAST(_self, ForStatement());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(ForStatement, void *, dtor, (void *_self), (_self))
{
  struct ForStatement *self = O_CAST(_self, ForStatement());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(ForStatement, Statement);
O_OBJECT_METHOD(ForStatement, ctor);
O_OBJECT_METHOD(ForStatement, dtor);
O_END_OBJECT
