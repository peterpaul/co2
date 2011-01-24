#include "co2/Expression.h"
#include "co2/Type.h"
#include "co2/io.h"

#define O_SUPER CompileObject()

O_IMPLEMENT (Expression, void *, ctor, (void *_self, va_list * app))
{
  struct Expression *self = O_CAST (_self, Expression ());
  self = O_SUPER->ctor (self, app);
  return self;
}

O_IMPLEMENT (Expression, void *, dtor, (void *_self))
{
  struct Expression *self = O_CAST (_self, Expression ());
  O_BRANCH_CALL (self->type, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (Expression, void, generate, (void *_self))
{
  struct Expression *self = O_CAST (_self, Expression ());
  O_CALL (self, generate_left, true);
}

O_OBJECT (Expression, CompileObject);
O_OBJECT_METHOD (Expression, ctor);
O_OBJECT_METHOD (Expression, dtor);
O_OBJECT_METHOD (Expression, generate);
O_END_OBJECT 

/**
 * Generates an argument expression.
 */
void
Expression_generate_actual_argument (void *_arg, va_list * app)
{
  struct Expression *arg = O_CAST (_arg, Expression ());
  bool *is_first_arg = va_arg (*app, bool *);
  if (!*is_first_arg)
    {
      fprintf (out, ", ");
    }
  else
    {
      *is_first_arg = false;
    }
  O_CALL (arg, generate);
}
