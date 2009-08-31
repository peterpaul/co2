#include "FunctionCallExpression.h"
#include "RefList.h"
#include "io.h"

#define O_SUPER Expression()

O_IMPLEMENT(FunctionCallExpression, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct FunctionCallExpression * self = O_CAST(_self, FunctionCallExpression());
  self = O_SUPER->ctor(self, app);
  self->function = O_CAST(va_arg(*app, struct Expression *), Expression());
  O_CALL(self->function, retain);
  self->actual_arguments = O_CAST(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->actual_arguments, retain);
  return self;
}

O_IMPLEMENT(FunctionCallExpression, void *, dtor, (void *_self), (_self))
{
  struct FunctionCallExpression *self = O_CAST(_self, FunctionCallExpression());
  O_CALL(self->function, release);
  O_CALL(self->actual_arguments, release);
  return O_SUPER->dtor(self);
}

void FunctionCallExpression_generate_actual_arguments(void *_arg, va_list *app)
{
  struct Expression *arg = O_CAST(_arg, Expression());
  bool * is_first_arg = va_arg(*app, bool *);
  if (!*is_first_arg) 
    {
      fprintf(out, ", ");
    }
  else
    {
      *is_first_arg = false;
    }
  O_CALL(arg, generate);
}

O_IMPLEMENT(FunctionCallExpression, void, generate, (void *_self), (_self))
{
  struct FunctionCallExpression *self = O_CAST(_self, FunctionCallExpression());
  bool is_first_arg = true;
  O_CALL(self->function, generate);
  fprintf(out, "(");
  O_CALL(self->actual_arguments, map_args, FunctionCallExpression_generate_actual_arguments, &is_first_arg);
  fprintf(out, ")");
}

O_OBJECT(FunctionCallExpression, Expression);
O_OBJECT_METHOD(FunctionCallExpression, ctor);
O_OBJECT_METHOD(FunctionCallExpression, dtor);
O_OBJECT_METHOD(FunctionCallExpression, generate);
O_END_OBJECT
