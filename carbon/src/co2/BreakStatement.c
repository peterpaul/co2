#include "co2/BreakStatement.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT(BreakStatement, void *, ctor, (void *_self, va_list *app))
{
  struct BreakStatement * self = O_CAST(_self, BreakStatement());
  self = O_SUPER->ctor(self, app);
  return self;
}

O_IMPLEMENT(BreakStatement, void *, dtor, (void *_self))
{
  struct BreakStatement *self = O_CAST(_self, BreakStatement());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(BreakStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct BreakStatement *self = O_CAST(_self, BreakStatement());
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(BreakStatement, void, type_check, (void *_self))
{
  struct BreakStatement *self = O_CAST(_self, BreakStatement());
}

O_IMPLEMENT(BreakStatement, void, generate, (void *_self))
{
  struct BreakStatement *self = O_CAST(_self, BreakStatement());
  fprintf (out, "break;\n");
}

O_OBJECT(BreakStatement, Statement);
O_OBJECT_METHOD(BreakStatement, ctor);
O_OBJECT_METHOD(BreakStatement, dtor);
O_OBJECT_METHOD(BreakStatement, accept);
O_OBJECT_METHOD(BreakStatement, type_check);
O_OBJECT_METHOD(BreakStatement, generate);
O_END_OBJECT
