#include "DeleteStatement.h"
#include "Expression.h"
#include "ObjectType.h"
#include "io.h"

#define O_SUPER Statement()

O_IMPLEMENT (DeleteStatement, void *, ctor, (void *_self, va_list * app))
{
  struct DeleteStatement *self = O_CAST (_self, DeleteStatement ());
  self = O_SUPER->ctor (self, app);
  self->expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (DeleteStatement, void *, dtor, (void *_self))
{
  struct DeleteStatement *self = O_CAST (_self, DeleteStatement ());
  O_CALL (self->expr, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (DeleteStatement, void, generate, (void *_self))
{
  struct DeleteStatement *self = O_CAST (_self, DeleteStatement ());
  /* TODO when self->expr->type == ArrayType, then free, else delete object */
  if (self->expr->type && o_is_of (self->expr->type, ObjectType ()))
    {
      fprintf (out, "O_CALL (");
      O_CALL (self->expr, generate);
      fprintf (out, ", delete);\n");
    }
  else
    {
      fprintf (out, "free (");
      O_CALL (self->expr, generate);
      fprintf (out, ");\n");
    }
}

O_IMPLEMENT (DeleteStatement, void, type_check, (void *_self))
{
  struct DeleteStatement *self = O_CAST (_self, DeleteStatement ());
  O_CALL (self->expr, type_check);
}

O_OBJECT (DeleteStatement, Statement);
O_OBJECT_METHOD (DeleteStatement, ctor);
O_OBJECT_METHOD (DeleteStatement, dtor);
O_OBJECT_METHOD (DeleteStatement, generate);
O_OBJECT_METHOD (DeleteStatement, type_check);
O_END_OBJECT
