#include "NullExpression.h"
#include "io.h"
#include "PrimitiveType.h"
#include "ArrayType.h"
#include "grammar.tab.h"

#define O_SUPER TokenExpression()

O_IMPLEMENT(NullExpression, void *, ctor, (void *_self, va_list *app))
{
  struct NullExpression * self = O_CAST(_self, NullExpression());
  self = O_SUPER->ctor(self, app);
  return self;
}

O_IMPLEMENT(NullExpression, void *, dtor, (void *_self))
{
  struct NullExpression *self = O_CAST(_self, NullExpression());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(NullExpression, void, type_check, (void *_self))
{
  struct NullExpression *self = O_CAST(_self, NullExpression());
  struct Token *type_token = O_CALL_CLASS(Token(), new_ctor, _Token_ctor_from_token, self->token, "void", VOID);
  struct Type *void_type = O_CALL_CLASS(PrimitiveType(), new, type_token);
  struct ArrayType *void_ptr_type = O_CALL_CLASS(ArrayType(), new, void_type);
  self->type = O_CALL(void_ptr_type, retain);
}

O_IMPLEMENT(NullExpression, void, generate, (void *_self))
{
  struct NullExpression *self = O_CAST(_self, NullExpression());
  fprintf(out, "NULL");
}

O_OBJECT(NullExpression, TokenExpression);
O_OBJECT_METHOD(NullExpression, ctor);
O_OBJECT_METHOD(NullExpression, dtor);
O_OBJECT_METHOD(NullExpression, type_check);
O_OBJECT_METHOD(NullExpression, generate);
O_END_OBJECT
