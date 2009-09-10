#include "DeleteStatement.h"
#include "Expression.h"
#include "io.h"

#define O_SUPER Statement()

O_IMPLEMENT(DeleteStatement, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct DeleteStatement * self = O_CAST(_self, DeleteStatement());
  self = O_SUPER->ctor(self, app);
  self->expr = O_CAST(va_arg(*app, struct EXpression *), Expression());
  O_CALL(self->expr, retain);
  return self;
}

O_IMPLEMENT(DeleteStatement, void *, dtor, (void *_self), (_self))
{
  struct DeleteStatement *self = O_CAST(_self, DeleteStatement());
  O_CALL(self->expr, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(DeleteStatement, void, generate, (void *_self), (_self))
{
  struct DeleteStatement *self = O_CAST(_self, DeleteStatement());
  /* TODO when self->expr->type == ArrayType, then free, else delete object */
  fprintf(out, "free(");
  O_CALL(self->expr, generate);
  fprintf(out, ");\n");
}

O_OBJECT(DeleteStatement, Statement);
O_OBJECT_METHOD(DeleteStatement, ctor);
O_OBJECT_METHOD(DeleteStatement, dtor);
O_OBJECT_METHOD(DeleteStatement, generate);
O_END_OBJECT
