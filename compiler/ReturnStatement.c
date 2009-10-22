#include "ReturnStatement.h"
#include "io.h"

#define O_SUPER Statement()

O_IMPLEMENT(ReturnStatement, void *, ctor, (void *_self, va_list *app))
{
  struct ReturnStatement * self = O_CAST(_self, ReturnStatement());
  self = O_SUPER->ctor(self, app);
  self->expr = O_BRANCH_CAST(va_arg(*app, struct Expression *), Expression());
  O_BRANCH_CALL(self->expr, retain);
  return self;
}

O_IMPLEMENT(ReturnStatement, void *, dtor, (void *_self))
{
  struct ReturnStatement *self = O_CAST(_self, ReturnStatement());
  O_BRANCH_CALL(self->expr, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ReturnStatement, void, generate, (void *_self))
{
  struct ReturnStatement *self = O_CAST(_self, ReturnStatement());
  fprintf(out, "return ");
  O_BRANCH_CALL(self->expr, generate);
  fprintf(out, ";\n");
}

O_IMPLEMENT(ReturnStatement, void, type_check, (void *_self))
{
  struct ReturnStatement *self = O_CAST(_self, ReturnStatement());
  O_CALL(self->expr, type_check);
}

O_OBJECT(ReturnStatement, Statement);
O_OBJECT_METHOD(ReturnStatement, ctor);
O_OBJECT_METHOD(ReturnStatement, dtor);
O_OBJECT_METHOD(ReturnStatement, generate);
O_OBJECT_METHOD(ReturnStatement, type_check);
O_END_OBJECT
