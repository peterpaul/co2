#include "ForEachStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT (ForEachStatement, void *, ctor, (void *_self, va_list * app))
{
  struct ForEachStatement *self = O_CAST (_self, ForEachStatement ());
  self = O_SUPER->ctor (self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT (ForEachStatement, void *, dtor, (void *_self))
{
  struct ForEachStatement *self = O_CAST (_self, ForEachStatement ());
  /* TODO cleanup */
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ForEachStatement, void, accept, (void *_self, struct CompileObjectVisitor *visitor))
{
  struct ForEachStatement *self = O_CAST (_self, ForEachStatement ());
  /* TODO implement */
  O_CALL_IF (CompileObjectVisitor, visitor, visit, self);
}

O_OBJECT (ForEachStatement, Statement);
O_OBJECT_METHOD (ForEachStatement, ctor);
O_OBJECT_METHOD (ForEachStatement, dtor);
O_OBJECT_METHOD (ForEachStatement, accept);
O_END_OBJECT
