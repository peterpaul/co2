#include "CompoundStatement.h"
#include "io.h"

#define O_SUPER Statement()

O_IMPLEMENT(CompoundStatement, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct CompoundStatement * self = O_CAST(_self, CompoundStatement());
  self = O_SUPER->ctor(self, app);
  self->body = O_CAST(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->body, retain);
  return self;
}

O_IMPLEMENT(CompoundStatement, void *, dtor, (void *_self), (_self))
{
  struct CompoundStatement *self = O_CAST(_self, CompoundStatement());
  O_CALL(self->body, release);
  return O_SUPER->dtor(self);
}

void CompoundStatement_generate_body(void *_item)
{
  struct CompileObject *item = O_CAST(_item, CompileObject());
  O_CALL(item, generate);
}

O_IMPLEMENT(CompoundStatement, void, generate, (void *_self), (_self))
{
  struct CompoundStatement *self = O_CAST(_self, CompoundStatement());
  fprintf(out, "{\n");
  O_CALL(self->body, map, CompoundStatement_generate_body);
  fprintf(out, "}\n");
}

O_OBJECT(CompoundStatement, Statement);
O_OBJECT_METHOD(CompoundStatement, ctor);
O_OBJECT_METHOD(CompoundStatement, dtor);
O_OBJECT_METHOD(CompoundStatement, generate);
O_END_OBJECT
