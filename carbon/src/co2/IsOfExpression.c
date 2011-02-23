#include "co2/IsOfExpression.h"
#include "co2/Token.h"
#include "co2/PrimitiveType.h"
#include "co2/io.h"
#include "grammar.h"

#define O_SUPER Expression()

O_IMPLEMENT (IsOfExpression, void *, ctor, (void *_self, va_list *app))
{
  struct IsOfExpression * self = O_CAST (_self, IsOfExpression());
  self = O_SUPER->ctor (self, app);
  self->expr_to_check = O_RETAIN_ARG (Expression);
  self->class_token = O_RETAIN_ARG (Token);
  return self;
}

O_IMPLEMENT (IsOfExpression, void *, dtor, (void *_self))
{
  struct IsOfExpression *self = O_CAST (_self, IsOfExpression());
  O_CALL (self->expr_to_check, release);
  O_CALL (self->class_token, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (IsOfExpression, void, type_check, (void *_self))
{
  struct IsOfExpression *self = O_CAST (_self, IsOfExpression());
  O_CALL (self->expr_to_check, type_check);
  struct Token * int_token = O_CALL_CLASS(Token (), new_ctor, _Token_ctor_from_token, self->class_token, "int", INT);
  self->type = O_CALL_CLASS (PrimitiveType (), new, int_token);
  
  O_CALL (self->type, retain);

}

O_IMPLEMENT (IsOfExpression, void, generate, (void *_self))
{
  struct IsOfExpression *self = O_CAST (_self, IsOfExpression());
  fprintf (out, "o_is_of (");
  O_CALL (self->expr_to_check, generate);
  fprintf (out, ", ");
  O_CALL (self->class_token, generate);
  fprintf (out, " ())");
}

O_OBJECT (IsOfExpression, Expression);
O_OBJECT_METHOD (IsOfExpression, ctor);
O_OBJECT_METHOD (IsOfExpression, dtor);
O_OBJECT_METHOD (IsOfExpression, type_check);
O_OBJECT_METHOD (IsOfExpression, generate);
O_END_OBJECT
