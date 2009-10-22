#include "VarDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Expression.h"
#include "io.h"

#define O_SUPER Declaration()

O_IMPLEMENT(VarDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  self = O_SUPER->ctor(self, app);
  self->expr = O_BRANCH_CALL(va_arg(*app, struct Expression *), retain);
  return self;
}

O_IMPLEMENT(VarDeclaration, void, set_type, (void *_self, struct Type * type))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  self->type = O_CALL(type, retain);
}

O_IMPLEMENT(VarDeclaration, void, generate, (void *_self))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  O_CALL(self->type, generate);
  fprintf(out, " ");
  O_CALL(self->name, generate);
  if (self->expr)
    {
      fprintf(out, " = ");
      O_CALL(self->expr, generate);
    }
  fprintf(out, ";\n");
}

O_IMPLEMENT(VarDeclaration, void, type_check, (void *_self))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  if (self->expr)
    {
      O_CALL(self->expr, type_check);
      O_CALL(self->type, assert_compatible, self->expr->type);
    }
}

O_OBJECT(VarDeclaration, Declaration);
O_OBJECT_METHOD(VarDeclaration, ctor);
O_OBJECT_METHOD(VarDeclaration, set_type);
O_OBJECT_METHOD(VarDeclaration, generate);
O_OBJECT_METHOD(VarDeclaration, type_check);
O_END_OBJECT
