#include "ExpressionStatement.h"
#include "io.h"

#define O_SUPER Statement()

O_IMPLEMENT(ExpressionStatement, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ExpressionStatement * self = O_CAST(_self, ExpressionStatement());
  self = O_SUPER->ctor(self, app);
  self->expr = O_CAST(va_arg(*app, struct Expression *), Expression());
  O_CALL(self->expr, retain);
  return self;
}

O_IMPLEMENT(ExpressionStatement, void *, dtor, (void *_self), (_self))
{
  struct ExpressionStatement *self = O_CAST(_self, ExpressionStatement());
  O_CALL(self->expr, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ExpressionStatement, void, generate, (void *_self), (_self))
{
  struct ExpressionStatement *self = O_CAST(_self, ExpressionStatement());
  O_CALL(self->expr, generate);
  fprintf(out, ";\n");
}

O_OBJECT(ExpressionStatement, Statement);
O_OBJECT_METHOD(ExpressionStatement, ctor);
O_OBJECT_METHOD(ExpressionStatement, dtor);
O_OBJECT_METHOD(ExpressionStatement, generate);
O_END_OBJECT
