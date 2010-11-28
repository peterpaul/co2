#include "IfStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT (IfStatement, void *, ctor, (void *_self, va_list * app))
{
  struct IfStatement *self = O_CAST (_self, IfStatement ());
  self = O_SUPER->ctor (self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT (IfStatement, void *, dtor, (void *_self))
{
  struct IfStatement *self = O_CAST (_self, IfStatement ());
  /* TODO cleanup */
  return O_SUPER->dtor (self);
}

O_OBJECT (IfStatement, Statement);
O_OBJECT_METHOD (IfStatement, ctor);
O_OBJECT_METHOD (IfStatement, dtor);
O_END_OBJECT
