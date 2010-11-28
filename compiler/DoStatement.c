#include "DoStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT (DoStatement, void *, ctor, (void *_self, va_list * app))
{
  struct DoStatement *self = O_CAST (_self, DoStatement ());
  self = O_SUPER->ctor (self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT (DoStatement, void *, dtor, (void *_self))
{
  struct DoStatement *self = O_CAST (_self, DoStatement ());
  /* TODO cleanup */
  return O_SUPER->dtor (self);
}

O_OBJECT (DoStatement, Statement);
O_OBJECT_METHOD (DoStatement, ctor);
O_OBJECT_METHOD (DoStatement, dtor);
O_END_OBJECT
