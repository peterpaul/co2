#include "co2/DoStatement.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT (DoStatement, void *, ctor, (void *_self, va_list * app))
{
  struct DoStatement *self = O_CAST (_self, DoStatement ());
  self = O_SUPER->ctor (self, app);
  self->body = O_RETAIN_ARG (Statement);
  self->condition = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (DoStatement, void *, dtor, (void *_self))
{
  struct DoStatement *self = O_CAST (_self, DoStatement ());
  O_CALL (self->body, release);
  O_CALL (self->condition, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (DoStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct DoStatement *self = O_CAST (_self, DoStatement ());
  O_CALL (self->body, accept, visitor);
  O_CALL (self->condition, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (DoStatement, void, type_check, (void *_self))
{
  struct DoStatement *self = O_CAST (_self, DoStatement ());
  O_CALL (self->body, type_check);
  O_CALL (self->condition, type_check);
}

O_IMPLEMENT (DoStatement, void, generate, (void *_self))
{
  struct DoStatement *self = O_CAST (_self, DoStatement ());
  fprintf (out, "do\n");
  O_CALL (self->body, generate);
  fprintf (out, "while (");
  O_CALL (self->condition, generate);
  fprintf (out, ");\n");
}

O_OBJECT (DoStatement, Statement);
O_OBJECT_METHOD (DoStatement, ctor);
O_OBJECT_METHOD (DoStatement, dtor);
O_OBJECT_METHOD (DoStatement, accept);
O_OBJECT_METHOD (DoStatement, type_check);
O_OBJECT_METHOD (DoStatement, generate);
O_END_OBJECT
