#include "WhileStatement.h"

#define O_SUPER Statement()

O_IMPLEMENT (WhileStatement, void *, ctor, (void *_self, va_list * app))
{
  struct WhileStatement *self = O_CAST (_self, WhileStatement ());
  self = O_SUPER->ctor (self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT (WhileStatement, void *, dtor, (void *_self))
{
  struct WhileStatement *self = O_CAST (_self, WhileStatement ());
  /* TODO cleanup */
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (WhileStatement, void, accept, (void *_self, struct CompileObjectVisitor *visitor))
{
  struct WhileStatement *self = O_CAST (_self, WhileStatement ());
  /* TODO implement */
  O_CALL_IF (CompileObjectVisitor, visitor, visit, self);
}

O_OBJECT (WhileStatement, Statement);
O_OBJECT_METHOD (WhileStatement, ctor);
O_OBJECT_METHOD (WhileStatement, dtor);
O_OBJECT_METHOD (WhileStatement, accept);
O_END_OBJECT
