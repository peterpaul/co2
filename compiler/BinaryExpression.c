#include "BinaryExpression.h"
#include "Token.h"
#include "Type.h"
#include "ArrayType.h"
#include "io.h"

#define O_SUPER Expression()

O_IMPLEMENT(BinaryExpression, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct BinaryExpression * self = O_CAST(_self, BinaryExpression());
  self = O_SUPER->ctor(self, app);
  self->operand[0] = O_BRANCH_CAST(va_arg(*app, struct Expression *), Expression());
  self->operator = O_BRANCH_CAST(va_arg(*app, struct Token *), Token());
  self->operand[1] = O_BRANCH_CAST(va_arg(*app, struct Expression *), Expression());
  O_BRANCH_CALL(self->operand[0], retain);
  O_BRANCH_CALL(self->operand[1], retain);
  O_BRANCH_CALL(self->operator, retain);
  return self;
}

O_IMPLEMENT(BinaryExpression, void *, dtor, (void *_self), (_self))
{
  struct BinaryExpression *self = O_CAST(_self, BinaryExpression());
  O_BRANCH_CALL(self->operand[0], release);
  O_BRANCH_CALL(self->operand[1], release);
  O_BRANCH_CALL(self->operator, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(BinaryExpression, void, generate, (void *_self), (self))
{
  struct BinaryExpression *self = O_CAST(_self, BinaryExpression());
  switch (self->operator->type) 
    {
    case '.':
      O_CALL(self->operand[0], generate);
      if (self->is_method == true)
	{
	  fprintf(out, "->class");
	}
      fprintf(out, "->");
      O_CALL(self->operand[1], generate);
      break;
    case '[':
      O_CALL(self->operand[0], generate);
      O_CALL(self->operator, generate);
      O_CALL(self->operand[1], generate);
      fprintf(out, "]");
      break;
    default:
      O_CALL(self->operand[0], generate);
      O_CALL(self->operator, generate);
      O_CALL(self->operand[1], generate);
      break;
    }
}

O_IMPLEMENT(BinaryExpression, void, type_check, (void *_self), (self))
{
  struct BinaryExpression *self = O_CAST(_self, BinaryExpression());
  switch (self->operator->type) 
    {
    case '.':
      break;
    case '[':
      O_CALL(self->operand[0], type_check);
      struct ArrayType * array_type = o_cast(self->operand[0]->type, ArrayType());
      self->type = O_CALL(array_type->base_type, retain);
      break;
    default:
      O_CALL(self->operand[0], type_check);
      O_CALL(self->operand[1], type_check);
      O_CALL(self->operand[0]->type, assert_compatible, self->operand[1]->type);
      self->type = O_CALL(self->operand[0], retain);
      break;
    }
}

O_OBJECT(BinaryExpression, Expression);
O_OBJECT_METHOD(BinaryExpression, ctor);
O_OBJECT_METHOD(BinaryExpression, dtor);
O_OBJECT_METHOD(BinaryExpression, generate);
O_OBJECT_METHOD(BinaryExpression, type_check);
O_END_OBJECT
