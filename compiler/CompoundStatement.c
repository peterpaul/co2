#include "CompoundStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT(CompoundStatement, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct CompoundStatement * self = O_CAST(_self, CompoundStatement());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(CompoundStatement, void *, dtor, (void *_self), (_self))
{
  struct CompoundStatement *self = O_CAST(_self, CompoundStatement());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(CompoundStatement, Statement);
O_OBJECT_METHOD(CompoundStatement, ctor);
O_OBJECT_METHOD(CompoundStatement, dtor);
O_END_OBJECT
