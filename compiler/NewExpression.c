#include "NewExpression.h"
#include "Type.h"
#include "Expression.h"
#include "RefList.h"
#include "PrimitiveType.h"
#include "io.h"
#include "Token.h"

#define O_SUPER Expression()

O_IMPLEMENT(NewExpression, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct NewExpression * self = O_CAST(_self, NewExpression());
  self = O_SUPER->ctor(self, app);
  self->new_type = O_CAST(va_arg(*app, struct Type *), Type());
  O_CALL(self->new_type, retain);
  struct Object * arg = O_CAST(va_arg(*app, struct Object *), Object());
  if (o_is_of(arg, Expression()))
    {
      self->array_size = (struct Expression *)arg;
    }
  else if (o_is_of(arg, RefList()))
    {
      self->ctor_arguments = (struct RefList *)arg;
    }
  else
    {
      error(NULL, "expected expression or list");
    }
  return self;
}

O_IMPLEMENT(NewExpression, void *, dtor, (void *_self), (_self))
{
  struct NewExpression *self = O_CAST(_self, NewExpression());
  O_CALL(self->new_type, release);
  O_BRANCH_CALL(self->array_size, release);
  O_BRANCH_CALL(self->ctor_arguments, release);
  return O_SUPER->dtor(self);
}

void NewExpression_generate_ctor_argument(void *_arg)
{
  struct Expression * actual_arg = O_CAST(_arg, Expression());
  fprintf(out, ", ");
  O_CALL(actual_arg, generate);
}

O_IMPLEMENT(NewExpression, void, generate, (void *_self), (_self))
{
  struct NewExpression *self = O_CAST(_self, NewExpression());
  if (self->ctor_arguments)
    {
      fprintf(out, "O_CALL_CLASS(");
      struct PrimitiveType * type = o_cast(self->new_type, PrimitiveType());
      O_CALL(type->token, generate);
      fprintf(out, "(), new");
      O_CALL(self->ctor_arguments, map, NewExpression_generate_ctor_argument);
      fprintf(out, ")");
    }
  else if (self->array_size)
    {
      fprintf(out, "calloc(");
      O_CALL(self->array_size, generate);
      fprintf(out, ", sizeof(");
      O_CALL(self->new_type, generate);
      fprintf(out, "))");
    }
  else
    {
      /* TODO error message */
    }
}

O_OBJECT(NewExpression, Expression);
O_OBJECT_METHOD(NewExpression, ctor);
O_OBJECT_METHOD(NewExpression, dtor);
O_OBJECT_METHOD(NewExpression, generate);
O_END_OBJECT
