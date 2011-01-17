#include "ForStatement.h"
#include "Expression.h"
#include "io.h"

#define O_SUPER Statement()

O_IMPLEMENT (ForStatement, void *, ctor, (void *_self, va_list * app))
{
  struct ForStatement *self = O_CAST (_self, ForStatement ());
  self = O_SUPER->ctor (self, app);
  self->initialization = O_RETAIN_ARG(Expression);
  self->condition = O_RETAIN_ARG(Expression);
  self->iteration = O_RETAIN_ARG(Expression);
  self->body = O_RETAIN_ARG(Statement);
  return self;
}

O_IMPLEMENT (ForStatement, void *, dtor, (void *_self))
{
  struct ForStatement *self = O_CAST (_self, ForStatement ());
  O_CALL(self->initialization, release);
  O_CALL(self->condition, release);
  O_CALL(self->iteration, release);
  O_CALL(self->body, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ForStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ForStatement *self = O_CAST (_self, ForStatement ());
  O_CALL (self->initialization, accept, visitor);
  O_CALL (self->condition, accept, visitor);
  O_CALL (self->iteration, accept, visitor);
  O_CALL (self->body, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (ForStatement, void, type_check, (void *_self))
{
  struct ForStatement *self = O_CAST (_self, ForStatement ());
  O_CALL(self->initialization, type_check);
  O_CALL(self->condition, type_check);
  O_CALL(self->iteration, type_check);
  O_CALL(self->body, type_check);
}

O_IMPLEMENT (ForStatement, void, generate, (void *_self))
{
  struct ForStatement *self = O_CAST (_self, ForStatement ());
  fprintf(out, "for (");
  O_CALL(self->initialization, generate);
  fprintf(out, "; ");
  O_CALL(self->condition, generate);
  fprintf(out, "; ");
  O_CALL(self->iteration, generate);
  fprintf(out, ")\n");
  O_CALL(self->body, generate);
}

O_OBJECT (ForStatement, Statement);
O_OBJECT_METHOD (ForStatement, ctor);
O_OBJECT_METHOD (ForStatement, dtor);
O_OBJECT_METHOD (ForStatement, accept);
O_OBJECT_METHOD (ForStatement, type_check);
O_OBJECT_METHOD (ForStatement, generate);
O_END_OBJECT
