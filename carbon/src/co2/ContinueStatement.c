#include "co2/ContinueStatement.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT(ContinueStatement, void *, ctor, (void *_self, va_list *app))
{
  struct ContinueStatement * self = O_CAST(_self, ContinueStatement());
  self = O_SUPER->ctor(self, app);
  return self;
}

O_IMPLEMENT(ContinueStatement, void *, dtor, (void *_self))
{
  struct ContinueStatement *self = O_CAST(_self, ContinueStatement());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ContinueStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ContinueStatement *self = O_CAST(_self, ContinueStatement());
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(ContinueStatement, void, type_check, (void *_self))
{
  struct ContinueStatement *self = O_CAST(_self, ContinueStatement());
}

O_IMPLEMENT(ContinueStatement, void, generate, (void *_self))
{
  struct ContinueStatement *self = O_CAST(_self, ContinueStatement());
  fprintf (out, "break;\n");
}

O_OBJECT(ContinueStatement, Statement);
O_OBJECT_METHOD(ContinueStatement, ctor);
O_OBJECT_METHOD(ContinueStatement, dtor);
O_OBJECT_METHOD(ContinueStatement, accept);
O_OBJECT_METHOD(ContinueStatement, type_check);
O_OBJECT_METHOD(ContinueStatement, generate);
O_END_OBJECT
