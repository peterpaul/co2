#include "VarArgExpression.h"
#include "io.h"

#define O_SUPER Expression()

O_IMPLEMENT (VarArgExpression, void *, ctor, (void *_self, va_list * app))
{
  struct VarArgExpression *self = O_CAST (_self, VarArgExpression ());
  self = O_SUPER->ctor (self, app);
  self->va_arg_type = O_CAST (va_arg (*app, struct Type *), Type ());
  O_CALL (self->va_arg_type, retain);
  return self;
}

O_IMPLEMENT (VarArgExpression, void *, dtor, (void *_self))
{
  struct VarArgExpression *self = O_CAST (_self, VarArgExpression ());
  O_CALL (self->va_arg_type, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (VarArgExpression, void, type_check, (void *_self))
{
  struct VarArgExpression *self = O_CAST (_self, VarArgExpression ());
  O_CALL (self->va_arg_type, type_check);
  self->type = O_CALL (self->va_arg_type, retain);
}

O_IMPLEMENT (VarArgExpression, void, generate, (void *_self))
{
  struct VarArgExpression *self = O_CAST (_self, VarArgExpression ());
  fprintf (out, "va_arg(ap, ");
  O_CALL (self->type, generate);
  fprintf (out, ")");
}

O_OBJECT (VarArgExpression, Expression);
O_OBJECT_METHOD (VarArgExpression, ctor);
O_OBJECT_METHOD (VarArgExpression, dtor);
O_OBJECT_METHOD (VarArgExpression, type_check);
O_OBJECT_METHOD (VarArgExpression, generate);
O_END_OBJECT
